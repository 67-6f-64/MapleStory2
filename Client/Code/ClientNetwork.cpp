#include "stdafx.h"
#include "ClientNetwork.h"
#include "GameObject.h"
#include "Layer.h"

#include "Player.h"
#include "PartyFallPlayer.h"
#include "PartyFallItem.h"
#include "MazePlayer.h"
#include "BattleGroundPlayer.h"

#include "Chat.h"

IMPLEMENT_SINGLETON(CClientNetwork)

CClientNetwork::CClientNetwork()
	:m_bClientSetting(false)
{
}

CClientNetwork::~CClientNetwork()
{
}

void CClientNetwork::RecvData(void)
{
	while (nullptr != m_pInstance)
	{
		Engine::NETDATA RxData = RecvMsg();

		ErrorCheck();
	
		if (72 == RxData.chKey) // 제대로 된 값이 들어왔는지 확인하는것.
		{
			switch (RxData.eMessage)
			{
			case Engine::NETMSG_SERVER_CREATE_PLAYER:
				PlayerCreate(RxData);
				break;
	
			case Engine::NETMSG_SERVER_DELETE_PLAYER:
				PlayerDelete(RxData.iIndex);
				break;
	
			case Engine::NETMSG_SERVER_SET_INDEX:
				if (!m_bClientSetting)
				{
					//cout << "세팅전 클라이언트 번호 : " << g_iClientIndex << endl;
					g_iClientIndex = RxData.iIndex;		// 서버 접속시 한번만 실행
					//cout << "세팅후 클라이언트 번호 : " << g_iClientIndex + 1 << endl;

					m_bClientSetting = true;
				}				
				break;
	
			case Engine::NETMSG_SERVER_PLAYER_UPDATE:
				PlayerUpdate(RxData);
				break;
	
			case Engine::NETMSG_SERVER_BULLET_CREATE:
				CreateBullet(RxData);
				break;
	
			case Engine::NETMSG_SERVER_SCENE_CHANGE:
				// 씬 변경 서브메세지 = 씬타입
				SceneChange(RxData.wSubMessage);
				break;
	
			case Engine::NETMSG_SERVER_SEND_CHATTING:
				// 채팅 발생
				Chat(RxData);
				break;

			case Engine::NETMSG_SERVER_PARTYFALL_ITEM:
				SetFallItem(RxData);
				break;
			}
		}
	}
}

void CClientNetwork::PlayerCreate(Engine::NETDATA tData)
{
	if (nullptr == (*m_ppbSceneFlagArr))
		return;

	SetNetDataBuffer(WORD(Engine::FLAG_PLAYER_CREATE), tData);
	(*m_ppbSceneFlagArr)[Engine::FLAG_PLAYER_CREATE] = true;	
	
	++m_iPlayerSize;
}

void CClientNetwork::PlayerDelete(int iIndex)
{
	if (nullptr == m_pMapLayer)
		return;
	
	list<Engine::CGameObject*>::iterator iter = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
	list<Engine::CGameObject*>::iterator iter_end = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->end();
	
	for (; iter != iter_end; ++iter)
	{
		if ((*iter)->GetServerIndex() == iIndex)
		{
			// 스레드 동기화때문에 여기서 삭제하면 안됨
			/*Engine::Safe_Delete(*iter);
			m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->erase(iter);*/
			(*iter)->SetDead();
			--m_iPlayerSize;
			break;
		}
	}
}

void CClientNetwork::PlayerUpdate(Engine::NETDATA tData)
{
	if (nullptr == (*m_ppbSceneFlagArr))
		return;
	
	SetNetDataBuffer(WORD(Engine::FLAG_PLAYER_UPDATE), tData);
	(*m_ppbSceneFlagArr)[Engine::FLAG_PLAYER_UPDATE] = true;	
}

void CClientNetwork::Chat(Engine::NETDATA tData)
{
	if (nullptr == m_pMapLayer)
		return;
	
	Engine::CGameObject* pGameObject = (*m_pMapLayer)[LAYER_UI]->GetObjList(_T("Chat"))->back();

	dynamic_cast<CChat*>(pGameObject)->GetServerChat(tData);
}

void CClientNetwork::SetFallItem(Engine::NETDATA tData)
{
	if (nullptr == m_pMapLayer)
		return;

	// 플레이어가 아직 만들어지기 전이면 건너뛴다.
	/*if ((*m_pMapLayer).end() == (*m_pMapLayer).find(LAYER_GAMELOGIC))
		return;*/

	std::list<Engine::CGameObject*>::iterator iter = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("PartyFallItem"))->begin();
	std::list<Engine::CGameObject*>::iterator iter_end = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("PartyFallItem"))->end();

	for (int i = 0; i < tData.iIndex; ++i)
		++iter;
	 
	dynamic_cast<CPartyFallItem*>(*iter)->SetCollide(true);
}

void CClientNetwork::CreateBullet(Engine::NETDATA tData)
{
	if (nullptr == (*m_ppbSceneFlagArr))
		return;

	SetNetDataBuffer(WORD(Engine::FLAG_BULLET_CREATE), tData);
	(*m_ppbSceneFlagArr)[Engine::FLAG_BULLET_CREATE] = true;	
}

void CClientNetwork::ErrorCheck(void)
{
	int iErrorType = WSAGetLastError();
	switch (iErrorType)
	{
	case 0:
		// printf("정상\n");
		break;

	case WSAENOTCONN:	// 소켓이 연결되어 있지 않을 때
		printf("-- 1번 에러\n");
		break;

	case WSAESHUTDOWN:	// 소켓이 종료되었을 때
		printf("-- 2번 에러\n");
		break;

	case WSANOTINITIALISED:
		// 성공적인 WSAStartup 함수의 호출은이 기능을 사용하기 전에 발생해야합니다.
		printf("-- 3번 에러\n");
		break;

	case WSAENETDOWN:
		// 네트워크 서브 시스템에 오류가 발생했습니다.
		printf("-- 4번 에러\n");
		break;

	case WSAEFAULT:
		// 버피 파라미터는 완전히 사용자 주소 공간의 유효 부분에 포함되지 않는다.
		printf("-- 5번 에러\n");
		break;

	case WSAEINTR:
		// 차단)호출을 취소했습니다 WSACancelBlockingCall .
		printf("-- 6번 에러\n");
		break;

	case WSAEINPROGRESS:
		// 블럭킹 윈속 v1.1이 현재 진행 중이거나, 서비스 프로 바이더가 콜백 함수를 여전히 처리하고 있습니다.
		printf("-- 7번 에러\n");
		break;

	case WSAENETRESET:
		// 연결 지향 소켓의 경우, 
		// 이 오류는 연결이로 인해 파괴되었음을 나타냅니다.
		// 연결 유지 작업이 진행되는 동안 오류가 감지 활동.
		// 데이터 그램 소켓의 경우, 이 오류가 살 수있는 
		// 기간이 만료되었음을 나타냅니다.
		printf("-- 8번 에러\n");
		break;

	case WSAENOTSOCK:
		// 기술자는 소켓이 아니다.
		printf("-- 9번 에러\n");
		break;

	case WSAEOPNOTSUPP:
		// MSG_OOB가 지정 되었으나, 소켓 타입 SOCK_STREAM과 같은 
		// 스트림 스타일 아닌, OOB 데이터는이 소켓과 관련된 
		// 통신 도메인에서 지원되지 않습니다, 
		// 소켓이 단방향 만 작업을 보내 지원합니다.
		printf("-- 10번 에러\n");
		break;
	case WSAEWOULDBLOCK:
		// 소켓은 블로킹으로 표시되며, 
		// 수신 동작을 차단하는 것입니다.
		//printf(" - No Message\n");
		//printf("-- 11번 에러 : \n");
		break;

	case WSAEMSGSIZE:
		// 이 메시지는 지정된 버퍼에 비해 너무 크며 잘 렸습니다.
		printf("-- 11번 에러\n");
		break;

	case WSAEINVAL:
		// 소켓에 연결되지 않은 결합 , 또는 알 수없는 플래그가
		// 지정 된, 또는 MSG_OOB가 SO_OOBINLINE 사용하거나 
		// 사용과 소켓 지정 (전용 바이트 스트림 소켓에 대한) 
		// LEN이 제로 또는 부의이었다.
		printf("-- 12번 에러\n");
		break;

	case WSAECONNABORTED:
		// 가상 회로 인해 타임 아웃 또는 다른 고장을 종결시켰다.
		// 더 이상 사용할 수 같은 응용 프로그램은 소켓을 닫습니다 없습니다.
		printf("-- 13번 에러\n");
		break;

	case WSAETIMEDOUT:
		// 연결은 네트워크 장애로 인해 또는 
		// 피어 시스템이 응답하지 못했기 때문에 삭제되었습니다.
		printf("-- 14번 에러\n");
		break;

	case WSAECONNRESET:
		// 가상 회선은 경질 또는 불완전 닫기를 실행하는 
		// 원격 측에 의해 리셋되었다.
		// 더 이상 사용할 수 같은 응용 프로그램은 소켓을 닫습니다 없습니다.
		// 은 UDP 데이터 그램 소켓에서이 오류는 
		// 이전의 전송 작업은 ICMP "포트에 연결할 수 없음"메시지가 
		// 결과 것을 나타냅니다.
		printf("원격에서 연결을 끊었습니다.\n");
		break;

	default:
		printf("Unknown Error.\n");
		break;
	}
}

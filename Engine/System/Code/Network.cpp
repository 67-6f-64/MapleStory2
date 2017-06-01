#include "stdafx.h"
#include "Network.h"

#include "Engine_Include.h"
#include "Export_Function.h"
#include "Include.h"
#include "GameObject.h"
#include "Player.h"
#include "PartyFallPlayer.h"
#include "Layer.h"
#include "Chat.h"

IMPLEMENT_SINGLETON(CNetwork)

CNetwork::CNetwork()
:m_pMapLayer(nullptr)
,m_pDevice(nullptr)
,m_pbSceneChange(nullptr)
, m_pwSceneType(nullptr)
, m_iPlayerSize(1)
{
}

CNetwork::~CNetwork()
{
	Release();
}

HRESULT CNetwork::InitNetwork(void)
{
	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	// 소켓 생성
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	// 소켓 주소 정보
	SOCKADDR_IN servAddr = { 0 };
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_addr.s_addr = inet_addr("192.168.1.35");	// 탁성우 PC IP : 43 
															// 정승호 PC IP : 35

	servAddr.sin_port = htons(7272);

	// 소켓 접속
	if (connect(socket_client, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_client);
		MSG_BOX(_T("서버에 접속할 수 없습니다."));
		return SOCKET_ERROR;
	}

	// 원본소스는 여기서 멀티 스레드 실행하는 부분
	//CreateThread(NULL, 0, SendThread, (LPVOID)socket_client, 0, NULL);

	// 상태 출력
	//MSG_BOX(_T("서버에 접속되었습니다."));

	return S_OK;
}

void CNetwork::SendMsg(NETDATA tData)
{
	send(socket_client, (char*)&tData, sizeof(tData), 0);
}

NETDATA CNetwork::RecvMsg(void)
{
	char buffer[1024];
	NETDATA* RxData;

	int strLen = recv(socket_client, buffer, sizeof(buffer) - 1, 0);
	if (0 >= strLen)
	{
		closesocket(socket_client);
		return NETDATA();
	}

	buffer[strLen] = '\0';
	RxData = (NETDATA*)buffer;

	return *RxData;
}

void CNetwork::Release(void)
{
	// 소켓 해제
	closesocket(socket_client);
}

void CNetwork::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

void CNetwork::RecvData(void)
{
	while (nullptr != m_pInstance)
	{
		NETDATA RxData = RecvMsg();

		if (72 == RxData.chKey) // 제대로 된 값이 들어왔는지 확인하는것.
		{
			switch (RxData.eMessage)
			{
			case NETMSG_SERVER_CREATE_PLAYER:
				PlayerCreate(RxData);
				break;

			case NETMSG_SERVER_DELETE_PLAYER:
				PlayerDelete(RxData.iIndex);
				break;

			case NETMSG_SERVER_SET_INDEX:			
				g_iClientIndex = RxData.iIndex;		// 서버 접속시 한번만 실행
				break;

			case NETMSG_SERVER_PLAYER_UPDATE:
				PlayerUpdate(RxData);
				break;

			case NETMSG_SERVER_BULLET_CREATE:
				// CreateBullet(RxData.iIndex);
				break;

			case NETMSG_SERVER_SCENE_CHANGE:
				// 씬 변경 서브메세지 = 씬타입
				SceneChange(RxData.wSubMessage);
				break;

			case NETMSG_SERVER_SEND_CHATTING:
				// 채팅 발생
				Chat(RxData);
				break;

			case NETMSG_SERVER_PARTYFALL_CUBE:
				SetFallCube(RxData);
				break;
			}
		}
	}
}

void CNetwork::SetLayer(LPMAPLAYER pLayer)
{
	m_pMapLayer = pLayer;
}

void CNetwork::PlayerCreate(NETDATA tData)
{
	if (nullptr == m_pMapLayer)
		return;

	list<Engine::CGameObject*>::iterator iter = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
	list<Engine::CGameObject*>::iterator iter_end = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->end();

	for (; iter != iter_end; ++iter)
	{
		if ((*iter)->GetServerIndex() == tData.iIndex)		// 이미 해당 인덱스의 플레이어가 만들어져 있다면 만들 필요 없다.
			return;
	}

	// 인덱스가 일치하는 플레이어가 없으면 생성
	Engine::CGameObject*	pObj = NULL;

	// SCENE_LOGO, SCENE_STAGE, SCENE_PARTYROOM, SCENE_PARTYFALL
	switch (SCENEID(*m_pwSceneType))
	{
	case SCENE_STAGE:
		pObj = CPlayer::Create(m_pDevice);
		break;
	case SCENE_PARTYROOM:
		pObj = CPlayer::Create(m_pDevice);
		break;
	case SCENE_PARTYFALL:
		pObj = CPartyFallPlayer::Create(m_pDevice);
		break;

	case SCENE_PARTYMAZE:
		// 추가 요망
		break;
	}

	// 로고일 경우 pObj가 NULL이므로 플레이어 생성 하지 않는다.
	if (nullptr == pObj)
		return;

	pObj->SetServerIndex(tData.iIndex);

	((*m_pMapLayer)[LAYER_GAMELOGIC])->AddObject(_T("Player"), pObj);

	++m_iPlayerSize;
}

void CNetwork::PlayerDelete(int iIndex)
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

void CNetwork::PlayerUpdate(NETDATA tData)
{
	if (nullptr == m_pMapLayer)
		return;

	// 플레이어가 아직 만들어지기 전이면 건너뛴다.
	if ((*m_pMapLayer).end() == (*m_pMapLayer).find(LAYER_GAMELOGIC))
		return;

	list<Engine::CGameObject*>::iterator iter = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
	list<Engine::CGameObject*>::iterator iter_end = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->end();

	for (; iter != iter_end; ++iter)
	{
		if ((*iter)->GetServerIndex() == tData.iIndex)
		{
			dynamic_cast<CPlayer*>(*iter)->SetLocalData(tData);
			break;
		}
	}
}

void CNetwork::SceneChange(WORD wType)
{
	if (nullptr == m_pbSceneChange || nullptr == m_pwSceneType)
		return;

	// 씬 변경되므로 플레그 바꿔주고
	*m_pbSceneChange = true;
	// 변경될 타입도 바꿔줌
	*m_pwSceneType = wType;
}

void CNetwork::Chat(NETDATA tData)
{
	if (nullptr == m_pMapLayer)
		return;

	Engine::CGameObject* pGameObject = (*m_pMapLayer)[LAYER_UI]->GetObjList(_T("Chat"))->back();

	dynamic_cast<CChat*>(pGameObject)->GetServerChat(tData);
}

void CNetwork::SetFallCube(NETDATA tData)
{
	if (nullptr == m_pMapLayer)
		return;

	Engine::CGameObject* pObj = (*m_pMapLayer)[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->front();

	dynamic_cast<CPartyFallPlayer*>(pObj)->SetPartyFallCubeCollide(tData.iIndex);
}

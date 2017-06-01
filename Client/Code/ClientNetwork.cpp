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
	
		if (72 == RxData.chKey) // ����� �� ���� ���Դ��� Ȯ���ϴ°�.
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
					//cout << "������ Ŭ���̾�Ʈ ��ȣ : " << g_iClientIndex << endl;
					g_iClientIndex = RxData.iIndex;		// ���� ���ӽ� �ѹ��� ����
					//cout << "������ Ŭ���̾�Ʈ ��ȣ : " << g_iClientIndex + 1 << endl;

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
				// �� ���� ����޼��� = ��Ÿ��
				SceneChange(RxData.wSubMessage);
				break;
	
			case Engine::NETMSG_SERVER_SEND_CHATTING:
				// ä�� �߻�
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
			// ������ ����ȭ������ ���⼭ �����ϸ� �ȵ�
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

	// �÷��̾ ���� ��������� ���̸� �ǳʶڴ�.
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
		// printf("����\n");
		break;

	case WSAENOTCONN:	// ������ ����Ǿ� ���� ���� ��
		printf("-- 1�� ����\n");
		break;

	case WSAESHUTDOWN:	// ������ ����Ǿ��� ��
		printf("-- 2�� ����\n");
		break;

	case WSANOTINITIALISED:
		// �������� WSAStartup �Լ��� ȣ������ ����� ����ϱ� ���� �߻��ؾ��մϴ�.
		printf("-- 3�� ����\n");
		break;

	case WSAENETDOWN:
		// ��Ʈ��ũ ���� �ý��ۿ� ������ �߻��߽��ϴ�.
		printf("-- 4�� ����\n");
		break;

	case WSAEFAULT:
		// ���� �Ķ���ʹ� ������ ����� �ּ� ������ ��ȿ �κп� ���Ե��� �ʴ´�.
		printf("-- 5�� ����\n");
		break;

	case WSAEINTR:
		// ����)ȣ���� ����߽��ϴ� WSACancelBlockingCall .
		printf("-- 6�� ����\n");
		break;

	case WSAEINPROGRESS:
		// ��ŷ ���� v1.1�� ���� ���� ���̰ų�, ���� ���� ���̴��� �ݹ� �Լ��� ������ ó���ϰ� �ֽ��ϴ�.
		printf("-- 7�� ����\n");
		break;

	case WSAENETRESET:
		// ���� ���� ������ ���, 
		// �� ������ �����̷� ���� �ı��Ǿ����� ��Ÿ���ϴ�.
		// ���� ���� �۾��� ����Ǵ� ���� ������ ���� Ȱ��.
		// ������ �׷� ������ ���, �� ������ �� ���ִ� 
		// �Ⱓ�� ����Ǿ����� ��Ÿ���ϴ�.
		printf("-- 8�� ����\n");
		break;

	case WSAENOTSOCK:
		// ����ڴ� ������ �ƴϴ�.
		printf("-- 9�� ����\n");
		break;

	case WSAEOPNOTSUPP:
		// MSG_OOB�� ���� �Ǿ�����, ���� Ÿ�� SOCK_STREAM�� ���� 
		// ��Ʈ�� ��Ÿ�� �ƴ�, OOB �����ʹ��� ���ϰ� ���õ� 
		// ��� �����ο��� �������� �ʽ��ϴ�, 
		// ������ �ܹ��� �� �۾��� ���� �����մϴ�.
		printf("-- 10�� ����\n");
		break;
	case WSAEWOULDBLOCK:
		// ������ ���ŷ���� ǥ�õǸ�, 
		// ���� ������ �����ϴ� ���Դϴ�.
		//printf(" - No Message\n");
		//printf("-- 11�� ���� : \n");
		break;

	case WSAEMSGSIZE:
		// �� �޽����� ������ ���ۿ� ���� �ʹ� ũ�� �� �Ƚ��ϴ�.
		printf("-- 11�� ����\n");
		break;

	case WSAEINVAL:
		// ���Ͽ� ������� ���� ���� , �Ǵ� �� ������ �÷��װ�
		// ���� ��, �Ǵ� MSG_OOB�� SO_OOBINLINE ����ϰų� 
		// ���� ���� ���� (���� ����Ʈ ��Ʈ�� ���Ͽ� ����) 
		// LEN�� ���� �Ǵ� �����̾���.
		printf("-- 12�� ����\n");
		break;

	case WSAECONNABORTED:
		// ���� ȸ�� ���� Ÿ�� �ƿ� �Ǵ� �ٸ� ������ ������״�.
		// �� �̻� ����� �� ���� ���� ���α׷��� ������ �ݽ��ϴ� �����ϴ�.
		printf("-- 13�� ����\n");
		break;

	case WSAETIMEDOUT:
		// ������ ��Ʈ��ũ ��ַ� ���� �Ǵ� 
		// �Ǿ� �ý����� �������� ���߱� ������ �����Ǿ����ϴ�.
		printf("-- 14�� ����\n");
		break;

	case WSAECONNRESET:
		// ���� ȸ���� ���� �Ǵ� �ҿ��� �ݱ⸦ �����ϴ� 
		// ���� ���� ���� ���µǾ���.
		// �� �̻� ����� �� ���� ���� ���α׷��� ������ �ݽ��ϴ� �����ϴ�.
		// �� UDP ������ �׷� ���Ͽ����� ������ 
		// ������ ���� �۾��� ICMP "��Ʈ�� ������ �� ����"�޽����� 
		// ��� ���� ��Ÿ���ϴ�.
		printf("���ݿ��� ������ �������ϴ�.\n");
		break;

	default:
		printf("Unknown Error.\n");
		break;
	}
}

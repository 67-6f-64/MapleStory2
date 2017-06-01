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
	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	// ���� ����
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	// ���� �ּ� ����
	SOCKADDR_IN servAddr = { 0 };
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_addr.s_addr = inet_addr("192.168.1.35");	// Ź���� PC IP : 43 
															// ����ȣ PC IP : 35

	servAddr.sin_port = htons(7272);

	// ���� ����
	if (connect(socket_client, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_client);
		MSG_BOX(_T("������ ������ �� �����ϴ�."));
		return SOCKET_ERROR;
	}

	// �����ҽ��� ���⼭ ��Ƽ ������ �����ϴ� �κ�
	//CreateThread(NULL, 0, SendThread, (LPVOID)socket_client, 0, NULL);

	// ���� ���
	//MSG_BOX(_T("������ ���ӵǾ����ϴ�."));

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
	// ���� ����
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

		if (72 == RxData.chKey) // ����� �� ���� ���Դ��� Ȯ���ϴ°�.
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
				g_iClientIndex = RxData.iIndex;		// ���� ���ӽ� �ѹ��� ����
				break;

			case NETMSG_SERVER_PLAYER_UPDATE:
				PlayerUpdate(RxData);
				break;

			case NETMSG_SERVER_BULLET_CREATE:
				// CreateBullet(RxData.iIndex);
				break;

			case NETMSG_SERVER_SCENE_CHANGE:
				// �� ���� ����޼��� = ��Ÿ��
				SceneChange(RxData.wSubMessage);
				break;

			case NETMSG_SERVER_SEND_CHATTING:
				// ä�� �߻�
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
		if ((*iter)->GetServerIndex() == tData.iIndex)		// �̹� �ش� �ε����� �÷��̾ ������� �ִٸ� ���� �ʿ� ����.
			return;
	}

	// �ε����� ��ġ�ϴ� �÷��̾ ������ ����
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
		// �߰� ���
		break;
	}

	// �ΰ��� ��� pObj�� NULL�̹Ƿ� �÷��̾� ���� ���� �ʴ´�.
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
			// ������ ����ȭ������ ���⼭ �����ϸ� �ȵ�
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

	// �÷��̾ ���� ��������� ���̸� �ǳʶڴ�.
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

	// �� ����ǹǷ� �÷��� �ٲ��ְ�
	*m_pbSceneChange = true;
	// ����� Ÿ�Ե� �ٲ���
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

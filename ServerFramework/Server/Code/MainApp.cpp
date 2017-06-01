#include "stdafx.h"
#include "MainApp.h"
#include "TimeMgr.h"

CMainApp::CMainApp()
	: m_wSceneType(1)
	, m_iClient(1)
	, m_iFps(0)
	, m_fTime(0.f)
	, m_iClientID(1)
{
}

CMainApp::~CMainApp()
{
	Release();
}

int CMainApp::InitApp(void)
{
	m_iFps = 0;
	m_fTime = 0.f;
	m_TimeMgr.InitTime();

	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	// ����� ��Ʈ ��ȣ
	m_PORT = 7272;

	ZeroMemory(m_socket_array, sizeof(SOCKET)*MAX_SOCKET);
	ZeroMemory(m_hEvent_array, sizeof(HANDLE)*MAX_SOCKET);

	// ���� ���� ����
	m_socket_array[0] = socket(AF_INET, SOCK_STREAM, 0);

	// ���� �ּ� ���� �ۼ�
	m_servAddr.sin_family = AF_INET;
	m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_servAddr.sin_port = htons(m_PORT);

	// ���� ���ε�
	if (bind(m_socket_array[0], (sockaddr*)&m_servAddr, sizeof(m_servAddr)) == SOCKET_ERROR)
	{
		closesocket(m_socket_array[0]);
		return -1;
	}

	// ���� ���
	if (listen(m_socket_array[0], SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(m_socket_array[0]);
		printf("listen() Error\n");
		return -1;
	}

	// �̺�Ʈ �ڵ� ����
	for (int i = 0; i < MAX_SOCKET; ++i)
	{
		m_hEvent_array[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hEvent_array[i] == INVALID_HANDLE_VALUE)
		{
			closesocket(m_socket_array[0]);
			printf("���� �̺�Ʈ ���� ����\n");
			return -1;
		}
	}

	// ��� ���� �̺�Ʈ �ڵ� ����
	if (WSAEventSelect(m_socket_array[0], m_hEvent_array[0], FD_ACCEPT) == SOCKET_ERROR)
	{
		closesocket(m_socket_array[0]);
		CloseHandle(m_hEvent_array[0]);
		printf("���� �̺�Ʈ ���� ����");
		return -1;
	}

	// ���� ���
	printf("Ŭ���̾�Ʈ ������ ��ٸ��� �ֽ��ϴ�.\n");

	// ������ �̺�Ʈ �ڵ� ����
	m_iClient = 1;
	// ������ �������ִ� �� Ÿ�� ����
	m_wSceneType = 1;	// ����Ʈ : SCENE_STAGE

	return 0;
}

void CMainApp::Update(void)
{
	//Sleep(1);
	FPSCheck();
	m_TimeMgr.SetTime();

	// Ŭ���̾�Ʈ ���� ����
	SOCKADDR_IN clntAddr;
	int clntLen = sizeof(clntAddr);
	SOCKET socket_client = accept(m_socket_array[0], (SOCKADDR*)&clntAddr, &clntLen);
	int iCheck = (int)socket_client;

	if (0 > iCheck)
	{
		//printf("accept() failed\n");
		return;
	}

	// �� �迭 �˻�
	m_iClientID = SearchEmtySocketArray();

	// ���ӽ� �̺�Ʈ
	Connected(&socket_client, m_iClientID);
}

void CMainApp::ClientSocketThread(void)
{
	int index = m_iClientID;
	const int iSize = sizeof(NETDATA);
	char buffer[iSize] = { 0, };

	while (1)
	{
		/*DWORD dwObject = WaitForMultipleObjectsEx(m_iClient, m_hEvent_array, FALSE, INFINITE, 0);

		if (dwObject == INFINITE)
		continue;

		if (dwObject == WAIT_OBJECT_0)
		continue;*/

		memset(buffer, 0, sizeof(buffer));
		int cnt = recv(m_socket_array[index], buffer, iSize, 0);

		if (0 >= cnt)	// Ŭ���̾�Ʈ ���� ���� ��
		{
			if (GetConnectCheck(iSize, index))
				continue;
			else
				return;
		}

		NETDATA* pRxData = NULL;
		pRxData = (NETDATA*)buffer;

		// �޼����� �޾��� �� �޼����� ���� ����
		RecvData(pRxData, index);
	}
}

bool CMainApp::GetConnectCheck(int iSize, int iIndex)
{
	int iErrorType = WSAGetLastError();
	switch (iErrorType)
	{
	case WSAENOTCONN:	// ������ ����Ǿ� ���� ���� ��
		DisConnectedCheck(iSize, iIndex);
		printf("-- 1�� ����\n");
		return false;

	case WSAESHUTDOWN:	// ������ ����Ǿ��� ��
		DisConnectedCheck(iSize, iIndex);
		printf("-- 2�� ����\n");
		return false;

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
		DisConnectedCheck(iSize, iIndex);
		return false;

	default:
		printf("Unknown Error.\n");
		DisConnectedCheck(iSize, iIndex);
		return false;
	}

	return true;
}

CMainApp * CMainApp::Create(void)
{	
	CMainApp*	pMainApp = new CMainApp;

	if (0 != pMainApp->InitApp())
	{
		delete pMainApp;
		pMainApp = nullptr;
	}

	return pMainApp;
}

void CMainApp::Release(void)
{
	// ���� ���� ����
	closesocket(m_socket_array[0]);
	CloseHandle(m_hEvent_array[0]);
}

int CMainApp::SearchEmtySocketArray(void)
{
	int index = -1;

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		if (m_socket_array[c] == 0)
		{
			index = c;
			break;
		}
	}

	return index;
}

int CMainApp::ClientCount(void)
{
	int iCnt = 0;

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		if (m_socket_array[c] == 0)
			continue;

		++iCnt;
	}

	return iCnt;
}



void CMainApp::Connected(SOCKET* pSocket, int iIndex)
{
	// �ϳ��� �����ߴٸ�
	if (0 < iIndex)
	{
		// Ŭ���̾�Ʈ �̺�Ʈ �ڵ� ����
		if (WSAEventSelect(*pSocket, m_hEvent_array[iIndex], FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			printf("Ŭ���̾�Ʈ �̺�Ʈ ���� ����.\n");
			return;
		}

		printf("%d�� -> Ŭ���̾�Ʈ ����\n", iIndex);

		// �迭�� ���� ����
		m_socket_array[iIndex] = *pSocket;
		m_iClient = max(m_iClient, iIndex + 1);

		// Ŭ�� ���ӽ� �ش� Ŭ���̾�Ʈ�� �ε��� ����
		// �ϴ� �ش� Ŭ���̾�Ʈ�� �ε��� ����
		NETDATA sendData;
		sendData.chKey = 72;
		sendData.eMessage = NETMSG_SERVER_SET_INDEX;
		sendData.iIndex = iIndex - 1;
		send(m_socket_array[iIndex], (char*)&sendData, sizeof(sendData), 0);

		// �ش� Ŭ���̾�Ʈ���� 1:1 ����� ���� ��Ƽ������ ����
		std::packaged_task<void()> task(std::bind(&CMainApp::ClientSocketThread, this));
		std::thread temp(move(task));
		temp.detach();
	}
	else
	{
		printf("�� �̻� ������ ������ �� �����ϴ�.\n");
		closesocket(*pSocket);
	}
}

void CMainApp::DisConnectedCheck(int iSize, int iIndex)
{
	DWORD dwTmp;
	char szBuffer[64] = { 0, };
	wsprintfA(szBuffer, "%d�� �����: ", iIndex);
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), szBuffer, strlen(szBuffer), &dwTmp, NULL);

	// Ŭ���̾�Ʈ ���� ����
	closesocket(m_socket_array[iIndex]);
	printf("Ŭ���̾�Ʈ ���� ����.. \n");

	// ���⼭ ������ ���ϵ鿡�� �ش� Ŭ�� �������� �ߴٰ� �˷��ְ�,
	// �÷��̾� ����
	for (int i = 1; i < MAX_SOCKET; ++i)
	{
		if (m_socket_array[i] != 0 && i != iIndex)
		{
			// ���Ź��� Ŭ���̾�Ʈ�� �����ϰ� ���� ó��
			NETDATA sendData;
			sendData.chKey = 72;
			sendData.eMessage = NETMSG_SERVER_DELETE_PLAYER;
			sendData.iIndex = iIndex - 1;
			send(m_socket_array[i], (char*)&sendData, sizeof(sendData), 0);
		}
	}

	// �迭�� ���� ����
	m_socket_array[iIndex] = 0;
}

void CMainApp::RecvData(NETDATA * pData, int iIndex)
{
	switch (pData->eMessage)
	{
		// Ŭ���̾�Ʈ�� �÷��̾� Transform ������ ����
	case NETMSG_CLIENT_PLAYER_NOTIPY:
		PlayerUpdate(pData, iIndex);
		break;

		// �ҷ� - ���� ����
	case NETMSG_CLIENT_BULLET_CREATE_NOTIPY:
		BulletCreate(pData, iIndex);
		break;

		// �÷��̾� ������û�� ����
	case NETMSG_CLIENT_CREATE_PLAYER:
		printf("\n[Ŭ��] -- %d �� Ŭ���̾�Ʈ�κ��� �ش� �÷��̾� ������û --\n", iIndex);
		PlayerCreate(pData, iIndex);
		break;

		// ��� Ŭ���̾�Ʈ�鿡�� �� ���� ����
	case NETMSG_CLIENT_SCENE_CHANGE:
		SceneChangeAll(pData);
		break;

		// Ŭ���̾�Ʈ�� �ΰ� ������ �� �� �ѹ� �����
		// ������ ����Ǿ� �ִ� �������� ��� ������ Ŭ���̾�Ʈ�� �Ѱ��� (����Ʈ : SCENE_STAGE)
	case NETMSG_CLIENT_SCENE_CHECK:
		SceneCheck(pData, iIndex);
		break;

		// ��ü Ŭ���̾�Ʈ���� ���Ź��� �޼����� �ѷ��ش�
	case NETMSG_CLIENT_SEND_CHATTING:
		SendChatting(pData, iIndex);
		break;

		// PartyFall ������������ �÷��̾�� �浹�� ť���� �ε����� 
		// ��� Ŭ���̾�Ʈ�� �˷��ش�
	//case NETMSG_CLIENT_PARTYFALL_CUBE:
	//	//printf("%d �� �����: ť�� �ε��� ( %d )\n", iIndex, pData->iIndex);
	//	SendFallCube(pData);
	//	break;

		// PartyFall ������������ �÷��̾�� �浹�� �������� �ε�����
		// ��� Ŭ���̾�Ʈ�� �˷��ش�.
	case NETMSG_CLIENT_PARTYFALL_ITEM:
		SendFallItem(pData, iIndex);
		break;
	}
}

void CMainApp::PlayerUpdate(NETDATA * pData, int iIndex)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = pData->chKey;
	tData.eMessage = NETMSG_SERVER_PLAYER_UPDATE;
	tData.iIndex = iIndex - 1;
	tData.matWold = pData->matWold;
	tData.iTexture = pData->iTexture;
	tData.dwFrame = pData->dwFrame;
	tData.wSubMessage = pData->wSubMessage;

	SendOther(&tData, iIndex);
}

void CMainApp::PlayerCreate(NETDATA * pData, int iIndex)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = pData->chKey;
	tData.eMessage = NETMSG_SERVER_CREATE_PLAYER;

	// ���ӵǾ��ִ� Ŭ���̾�Ʈ ������
	int iCnt = ClientCount();

	// �ش� Ŭ���̾�Ʈ���� ������ Ŭ���̾�Ʈ �÷��̾ ����
	for (int i = 1; i < MAX_SOCKET; ++i)
	{
		if (m_socket_array[i] != 0 && i != iIndex)
		{
			//Sleep(30);
			tData.iIndex = iCnt;

			printf("[����] - %d �� Ŭ�� : 0�� ~ %d�� ���� �÷��̾� ���� -\n", iIndex, iCnt);
			SendOne(&tData, iIndex);
		}
	}

	tData.iIndex = iCnt;
	//SendOther(&tData, iIndex);

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		if (m_socket_array[c] != 0 && c != iIndex)
		{
			printf("[����] - %d �� Ŭ�� : %d�� �÷��̾� ���� -\n", c, iIndex);
			// ���Ź��� Ŭ���̾�Ʈ�� �����ϰ� ���� ó��
			send(m_socket_array[c], (char*)&tData, sizeof(NETDATA), 0);
		}
	}
}

void CMainApp::BulletCreate(NETDATA * pData, int iIndex)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = pData->chKey;
	tData.eMessage = NETMSG_SERVER_BULLET_CREATE;
	tData.iIndex = iIndex - 1;

	SendAll(&tData);
}

void CMainApp::SceneChangeAll(NETDATA * pData)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = 72;
	tData.eMessage = NETMSG_SERVER_SCENE_CHANGE;

	m_wSceneType = pData->wSubMessage;
	tData.wSubMessage = m_wSceneType;

	//SendAll(&tData);

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		// ��ü ����
		// �޼��� ���� ������ ���� ������ �ɾ��ش�
		//Sleep(20);
		send(m_socket_array[c], (char*)&tData, sizeof(NETDATA), 0);
	}
}

void CMainApp::SceneCheck(NETDATA * pData, int iIndex)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = 72;
	tData.eMessage = NETMSG_SERVER_SCENE_CHANGE;
	tData.wSubMessage = m_wSceneType;		// ���⼭�� �� Ÿ���� �ȴ�.
	tData.iIndex = iIndex - 1;

	SendOne(&tData, iIndex);
}

void CMainApp::SendChatting(NETDATA * pData, int iIndex)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = 72;
	tData.eMessage = NETMSG_SERVER_SEND_CHATTING;
	tData.iIndex = iIndex - 1;
	strcpy_s(tData.szMessage, pData->szMessage);

	//SendAll(&tData);

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		// ��ü ����
		//Sleep(30);		// ä���� ���� ���� �ɾ��ش�
		send(m_socket_array[c], (char*)&tData, sizeof(NETDATA), 0);
	}
}

//void CMainApp::SendFallCube(NETDATA * pData)
//{
//	NETDATA tData;
//	ZeroMemory(&tData, sizeof(NETDATA));
//	tData.chKey = 72;
//	tData.eMessage = NETMSG_SERVER_PARTYFALL_CUBE;
//	tData.iIndex = pData->iIndex;
//
//	SendAll(&tData);
//}

void CMainApp::SendFallItem(NETDATA * pData, int iIndex)
{
	NETDATA tData;
	ZeroMemory(&tData, sizeof(NETDATA));
	tData.chKey = 72;
	tData.eMessage = NETMSG_SERVER_PARTYFALL_ITEM;
	tData.iIndex = pData->iIndex;

	//SendOther(&tData, iIndex);

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		if (m_socket_array[c] != 0 && c != iIndex)
		{
			//Sleep(30);
			// ���Ź��� Ŭ���̾�Ʈ�� �����ϰ� ���� ó��
			send(m_socket_array[c], (char*)&tData, sizeof(NETDATA), 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
void CMainApp::SendAll(NETDATA * pData)
{
	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		// ��ü ����
		send(m_socket_array[c], (char*)pData, sizeof(NETDATA), 0);
	}
}

void CMainApp::SendOne(NETDATA * pData, int iIndex)
{
	send(m_socket_array[iIndex], (char*)pData, sizeof(NETDATA), 0);
}

void CMainApp::SendOther(NETDATA * pData, int iIndex)
{
	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		if (m_socket_array[c] != 0 && c != iIndex)
		{
			// ���Ź��� Ŭ���̾�Ʈ�� �����ϰ� ���� ó��
			send(m_socket_array[c], (char*)pData, sizeof(NETDATA), 0);
		}
	}
}

void CMainApp::FPSCheck(void)
{
	char szFPS[64] = {};

	m_fTime += m_TimeMgr.GetTime();
	++m_iFps;

	sprintf_s(szFPS, "title Fps : %d", m_iFps);

	if (1.f < m_fTime)
	{
		system(szFPS);

		m_fTime = 0.f;
		m_iFps = 0;
	}
}

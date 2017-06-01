#include "Network.h"

#include "GameObject.h"
#include "Layer.h"

Engine::CNetwork::CNetwork()
: m_pMapLayer(nullptr)
, m_pDevice(nullptr)
, m_pbSceneChange(nullptr)
, m_pwSceneType(nullptr)
, m_pNull(nullptr)
, m_iPlayerSize(1)
, m_ppbSceneFlagArr(&m_pNull)
{
}

Engine::CNetwork::~CNetwork()
{
	Release();
}

HRESULT Engine::CNetwork::InitNetwork(void)
{
	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	// ���� ����
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	// ���� �ּ� ����
	SOCKADDR_IN servAddr = { 0 };
	servAddr.sin_family = AF_INET;							

	// Ź���� PC IP : 43 
	// ����ȣ PC IP : 35

	HANDLE m_hFile = CreateFile(L"../bin/Server.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	char Temp[32] = { 0, };

	DWORD dwByte = 0;

	ReadFile(m_hFile, Temp, sizeof(char)*32, &dwByte, NULL);

	//inet_pton(AF_INET, "192.168.1.43", &(servAddr.sin_addr));
	inet_pton(AF_INET, Temp, &(servAddr.sin_addr));
	//inet_pton(AF_INET, "127.0.0.1", &(servAddr.sin_addr));

	CloseHandle(m_hFile);

	servAddr.sin_port = htons(7272);

	// ���� ����
	if (connect(socket_client, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_client);
		MSG_BOX(L"������ ������ �� �����ϴ�.");
		return SOCKET_ERROR;
	}

	return S_OK;
}

void Engine::CNetwork::SendMsg(Engine::NETDATA tData)
{
	send(socket_client, (char*)&tData, sizeof(tData), 0);
}

Engine::NETDATA Engine::CNetwork::RecvMsg(void)
{
	const int iSize = sizeof(NETDATA);
	char buffer[iSize] = { 0, };
	NETDATA* RxData;

	int strLen = recv(socket_client, buffer, iSize, 0);
	if (0 >= strLen)
	{
		closesocket(socket_client);
		//std::cout << "������ ���� ����Ǿ����ϴ�." << endl;
		return NETDATA();
	}

	RxData = (NETDATA*)buffer;

	return *RxData;
}

void Engine::CNetwork::Release(void)
{
	// ���� ����
	closesocket(socket_client);

	// �÷��̾� �� ��ü �Ҵ� ����
	////////////////////////////////////////////////////////////////
	MAPPLAYERDATA::iterator playerIter = m_mapPlayerData.begin();
	MAPPLAYERDATA::iterator playerIter_end = m_mapPlayerData.end();

	for (; playerIter != playerIter_end; ++playerIter)
		Safe_Delete(playerIter->second);

	m_mapPlayerData.clear();
	m_mapNetData[WORD(FLAG_PLAYER_UPDATE)] = nullptr;	// ��ü �� �ڷ��� �÷��̾� �� �ּ����� null �ʱ�ȭ
														// ���ϸ� ������
	/////////////////////////////////////////////////////////////////

	MAPDATA::iterator iter = m_mapNetData.begin();
	MAPDATA::iterator iter_end = m_mapNetData.end();

	for (; iter != iter_end; ++iter)
		Safe_Delete(iter->second);

	m_mapNetData.clear();
}

void Engine::CNetwork::SetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

void Engine::CNetwork::SetLayer(LPMAPLAYER pLayer)
{
	m_pMapLayer = pLayer;
}

const void * Engine::CNetwork::GetMapNetData(WORD wKey)
{
	MAPDATA::iterator iter = m_mapNetData.find(wKey);

	if (iter == m_mapNetData.end())
	{
		//std::cout << "GetMapNetData(" << wKey << ")" << " - �������� ��ȯ" << endl;
		NULL_CHECK_RETURN(nullptr, nullptr);
		return nullptr;
	}

	return (iter->second);
}

void Engine::CNetwork::SceneChange(WORD wType)
{
	if (nullptr == m_pbSceneChange || nullptr == m_pwSceneType)
		return;

	// �� ����ǹǷ� �÷��� �ٲ��ְ�
	*m_pbSceneChange = true;
	// ����� Ÿ�Ե� �ٲ���
	*m_pwSceneType = wType;
}

void Engine::CNetwork::SetNetDataBuffer(WORD wKey, NETDATA tData)
{
	MAPDATA::iterator iter = m_mapNetData.find(wKey);

	if (FLAG_PLAYER_UPDATE == eFLAGID(wKey))
	{
		// �ش� �ε����� �����ϴ� �÷��̾������ �޸𸮰� �Ҵ�ȵǾ� ������ �Ҵ�
		MAPPLAYERDATA::iterator playerIter = m_mapPlayerData.find(tData.iIndex);
		if (playerIter == m_mapPlayerData.end())
			m_mapPlayerData[tData.iIndex] = new NETDATA();

		// ������Ʈ�� ������ ����
		memcpy_s(m_mapPlayerData[tData.iIndex], sizeof(NETDATA), &tData, sizeof(NETDATA));

		// �÷��̾� ������Ʈ�� �����Ͱ� ���� �ȵǾ� ������ ����
		if (iter == m_mapNetData.end())
			m_mapNetData[wKey] = &m_mapPlayerData;
		
		memcpy_s(m_mapNetData[wKey], sizeof(NETDATA)*m_mapPlayerData.size(), &m_mapPlayerData, sizeof(NETDATA)*m_mapPlayerData.size());
	}
	else
	{
		if (iter == m_mapNetData.end())
			m_mapNetData[wKey] = new NETDATA();

		memcpy_s(m_mapNetData[wKey], sizeof(NETDATA), &tData, sizeof(NETDATA));
	}	
}

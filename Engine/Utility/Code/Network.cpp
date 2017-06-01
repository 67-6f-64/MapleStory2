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
	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	// 소켓 생성
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	// 소켓 주소 정보
	SOCKADDR_IN servAddr = { 0 };
	servAddr.sin_family = AF_INET;							

	// 탁성우 PC IP : 43 
	// 정승호 PC IP : 35

	HANDLE m_hFile = CreateFile(L"../bin/Server.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	char Temp[32] = { 0, };

	DWORD dwByte = 0;

	ReadFile(m_hFile, Temp, sizeof(char)*32, &dwByte, NULL);

	//inet_pton(AF_INET, "192.168.1.43", &(servAddr.sin_addr));
	inet_pton(AF_INET, Temp, &(servAddr.sin_addr));
	//inet_pton(AF_INET, "127.0.0.1", &(servAddr.sin_addr));

	CloseHandle(m_hFile);

	servAddr.sin_port = htons(7272);

	// 소켓 접속
	if (connect(socket_client, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_client);
		MSG_BOX(L"서버에 접속할 수 없습니다.");
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
		//std::cout << "서버와 접속 종료되었습니다." << endl;
		return NETDATA();
	}

	RxData = (NETDATA*)buffer;

	return *RxData;
}

void Engine::CNetwork::Release(void)
{
	// 소켓 해제
	closesocket(socket_client);

	// 플레이어 맵 전체 할당 해제
	////////////////////////////////////////////////////////////////
	MAPPLAYERDATA::iterator playerIter = m_mapPlayerData.begin();
	MAPPLAYERDATA::iterator playerIter_end = m_mapPlayerData.end();

	for (; playerIter != playerIter_end; ++playerIter)
		Safe_Delete(playerIter->second);

	m_mapPlayerData.clear();
	m_mapNetData[WORD(FLAG_PLAYER_UPDATE)] = nullptr;	// 전체 맵 자료의 플레이어 맵 주소정보 null 초기화
														// 안하면 터진다
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
		//std::cout << "GetMapNetData(" << wKey << ")" << " - 널포인터 반환" << endl;
		NULL_CHECK_RETURN(nullptr, nullptr);
		return nullptr;
	}

	return (iter->second);
}

void Engine::CNetwork::SceneChange(WORD wType)
{
	if (nullptr == m_pbSceneChange || nullptr == m_pwSceneType)
		return;

	// 씬 변경되므로 플레그 바꿔주고
	*m_pbSceneChange = true;
	// 변경될 타입도 바꿔줌
	*m_pwSceneType = wType;
}

void Engine::CNetwork::SetNetDataBuffer(WORD wKey, NETDATA tData)
{
	MAPDATA::iterator iter = m_mapNetData.find(wKey);

	if (FLAG_PLAYER_UPDATE == eFLAGID(wKey))
	{
		// 해당 인덱스에 대응하는 플레이어데이터의 메모리가 할당안되어 있으면 할당
		MAPPLAYERDATA::iterator playerIter = m_mapPlayerData.find(tData.iIndex);
		if (playerIter == m_mapPlayerData.end())
			m_mapPlayerData[tData.iIndex] = new NETDATA();

		// 업데이트용 데이터 세팅
		memcpy_s(m_mapPlayerData[tData.iIndex], sizeof(NETDATA), &tData, sizeof(NETDATA));

		// 플레이어 업데이트용 데이터가 연결 안되어 있으면 연결
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

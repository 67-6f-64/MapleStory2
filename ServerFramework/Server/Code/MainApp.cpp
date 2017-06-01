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

	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	// 사용할 포트 번호
	m_PORT = 7272;

	ZeroMemory(m_socket_array, sizeof(SOCKET)*MAX_SOCKET);
	ZeroMemory(m_hEvent_array, sizeof(HANDLE)*MAX_SOCKET);

	// 대기용 소켓 생성
	m_socket_array[0] = socket(AF_INET, SOCK_STREAM, 0);

	// 소켓 주소 정보 작성
	m_servAddr.sin_family = AF_INET;
	m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_servAddr.sin_port = htons(m_PORT);

	// 소켓 바인드
	if (bind(m_socket_array[0], (sockaddr*)&m_servAddr, sizeof(m_servAddr)) == SOCKET_ERROR)
	{
		closesocket(m_socket_array[0]);
		return -1;
	}

	// 소켓 대기
	if (listen(m_socket_array[0], SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(m_socket_array[0]);
		printf("listen() Error\n");
		return -1;
	}

	// 이벤트 핸들 생성
	for (int i = 0; i < MAX_SOCKET; ++i)
	{
		m_hEvent_array[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_hEvent_array[i] == INVALID_HANDLE_VALUE)
		{
			closesocket(m_socket_array[0]);
			printf("서버 이벤트 생성 실패\n");
			return -1;
		}
	}

	// 대기 소켓 이벤트 핸들 설정
	if (WSAEventSelect(m_socket_array[0], m_hEvent_array[0], FD_ACCEPT) == SOCKET_ERROR)
	{
		closesocket(m_socket_array[0]);
		CloseHandle(m_hEvent_array[0]);
		printf("서버 이벤트 설정 실패");
		return -1;
	}

	// 상태 출력
	printf("클라이언트 접속을 기다리고 있습니다.\n");

	// 설정된 이벤트 핸들 갯수
	m_iClient = 1;
	// 서버가 가지고있는 씬 타입 정보
	m_wSceneType = 1;	// 디폴트 : SCENE_STAGE

	return 0;
}

void CMainApp::Update(void)
{
	//Sleep(1);
	FPSCheck();
	m_TimeMgr.SetTime();

	// 클라이언트 소켓 생성
	SOCKADDR_IN clntAddr;
	int clntLen = sizeof(clntAddr);
	SOCKET socket_client = accept(m_socket_array[0], (SOCKADDR*)&clntAddr, &clntLen);
	int iCheck = (int)socket_client;

	if (0 > iCheck)
	{
		//printf("accept() failed\n");
		return;
	}

	// 빈 배열 검색
	m_iClientID = SearchEmtySocketArray();

	// 접속시 이벤트
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

		if (0 >= cnt)	// 클라이언트 종료 했을 때
		{
			if (GetConnectCheck(iSize, index))
				continue;
			else
				return;
		}

		NETDATA* pRxData = NULL;
		pRxData = (NETDATA*)buffer;

		// 메세지를 받았을 때 메세지에 따라 동작
		RecvData(pRxData, index);
	}
}

bool CMainApp::GetConnectCheck(int iSize, int iIndex)
{
	int iErrorType = WSAGetLastError();
	switch (iErrorType)
	{
	case WSAENOTCONN:	// 소켓이 연결되어 있지 않을 때
		DisConnectedCheck(iSize, iIndex);
		printf("-- 1번 에러\n");
		return false;

	case WSAESHUTDOWN:	// 소켓이 종료되었을 때
		DisConnectedCheck(iSize, iIndex);
		printf("-- 2번 에러\n");
		return false;

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
	// 서버 소켓 해제
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
	// 하나라도 접속했다면
	if (0 < iIndex)
	{
		// 클라이언트 이벤트 핸들 설정
		if (WSAEventSelect(*pSocket, m_hEvent_array[iIndex], FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			printf("클라이언트 이벤트 설정 실패.\n");
			return;
		}

		printf("%d번 -> 클라이언트 접속\n", iIndex);

		// 배열에 소켓 저장
		m_socket_array[iIndex] = *pSocket;
		m_iClient = max(m_iClient, iIndex + 1);

		// 클라 접속시 해당 클라이언트에 인덱스 세팅
		// 일단 해당 클라이언트에 인덱스 세팅
		NETDATA sendData;
		sendData.chKey = 72;
		sendData.eMessage = NETMSG_SERVER_SET_INDEX;
		sendData.iIndex = iIndex - 1;
		send(m_socket_array[iIndex], (char*)&sendData, sizeof(sendData), 0);

		// 해당 클라이언트와의 1:1 통신을 위한 멀티스레드 생성
		std::packaged_task<void()> task(std::bind(&CMainApp::ClientSocketThread, this));
		std::thread temp(move(task));
		temp.detach();
	}
	else
	{
		printf("더 이상 서버에 접속할 수 없습니다.\n");
		closesocket(*pSocket);
	}
}

void CMainApp::DisConnectedCheck(int iSize, int iIndex)
{
	DWORD dwTmp;
	char szBuffer[64] = { 0, };
	wsprintfA(szBuffer, "%d번 사용자: ", iIndex);
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), szBuffer, strlen(szBuffer), &dwTmp, NULL);

	// 클라이언트 접속 종료
	closesocket(m_socket_array[iIndex]);
	printf("클라이언트 접속 종료.. \n");

	// 여기서 나머지 소켓들에게 해당 클라가 접속해제 했다고 알려주고,
	// 플레이어 삭제
	for (int i = 1; i < MAX_SOCKET; ++i)
	{
		if (m_socket_array[i] != 0 && i != iIndex)
		{
			// 수신받은 클라이언트를 제외하고 애코 처리
			NETDATA sendData;
			sendData.chKey = 72;
			sendData.eMessage = NETMSG_SERVER_DELETE_PLAYER;
			sendData.iIndex = iIndex - 1;
			send(m_socket_array[i], (char*)&sendData, sizeof(sendData), 0);
		}
	}

	// 배열에 소켓 제거
	m_socket_array[iIndex] = 0;
}

void CMainApp::RecvData(NETDATA * pData, int iIndex)
{
	switch (pData->eMessage)
	{
		// 클라이언트의 플레이어 Transform 정보를 갱신
	case NETMSG_CLIENT_PLAYER_NOTIPY:
		PlayerUpdate(pData, iIndex);
		break;

		// 불렛 - 수정 예정
	case NETMSG_CLIENT_BULLET_CREATE_NOTIPY:
		BulletCreate(pData, iIndex);
		break;

		// 플레이어 생성요청시 실행
	case NETMSG_CLIENT_CREATE_PLAYER:
		printf("\n[클라] -- %d 번 클라이언트로부터 해당 플레이어 생성요청 --\n", iIndex);
		PlayerCreate(pData, iIndex);
		break;

		// 모든 클라이언트들에게 씬 변경 지시
	case NETMSG_CLIENT_SCENE_CHANGE:
		SceneChangeAll(pData);
		break;

		// 클라이언트가 로고에 들어왔을 때 단 한번 실행됨
		// 서버에 저장되어 있는 씬정보를 방금 접속한 클라이언트에 넘겨줌 (디폴트 : SCENE_STAGE)
	case NETMSG_CLIENT_SCENE_CHECK:
		SceneCheck(pData, iIndex);
		break;

		// 전체 클라이언트에게 수신받은 메세지를 뿌려준다
	case NETMSG_CLIENT_SEND_CHATTING:
		SendChatting(pData, iIndex);
		break;

		// PartyFall 스테이지에서 플레이어와 충돌된 큐브의 인덱스를 
		// 모든 클라이언트에 알려준다
	//case NETMSG_CLIENT_PARTYFALL_CUBE:
	//	//printf("%d 번 사용자: 큐브 인덱스 ( %d )\n", iIndex, pData->iIndex);
	//	SendFallCube(pData);
	//	break;

		// PartyFall 스테이지에서 플레이어와 충돌된 아이템의 인덱스를
		// 모든 클라이언트에 알려준다.
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

	// 접속되어있는 클라이언트 사이즈
	int iCnt = ClientCount();

	// 해당 클라이언트에게 나머지 클라이언트 플레이어를 생성
	for (int i = 1; i < MAX_SOCKET; ++i)
	{
		if (m_socket_array[i] != 0 && i != iIndex)
		{
			//Sleep(30);
			tData.iIndex = iCnt;

			printf("[서버] - %d 번 클라 : 0번 ~ %d번 까지 플레이어 생성 -\n", iIndex, iCnt);
			SendOne(&tData, iIndex);
		}
	}

	tData.iIndex = iCnt;
	//SendOther(&tData, iIndex);

	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		if (m_socket_array[c] != 0 && c != iIndex)
		{
			printf("[서버] - %d 번 클라 : %d번 플레이어 생성 -\n", c, iIndex);
			// 수신받은 클라이언트를 제외하고 애코 처리
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
		// 전체 전송
		// 메세지 씹힘 방지를 위해 슬립을 걸어준다
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
	tData.wSubMessage = m_wSceneType;		// 여기서는 씬 타입이 된다.
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
		// 전체 전송
		//Sleep(30);		// 채팅할 때만 슬립 걸어준다
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
			// 수신받은 클라이언트를 제외하고 애코 처리
			send(m_socket_array[c], (char*)&tData, sizeof(NETDATA), 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
void CMainApp::SendAll(NETDATA * pData)
{
	for (int c = 1; c < MAX_SOCKET; ++c)
	{
		// 전체 전송
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
			// 수신받은 클라이언트를 제외하고 애코 처리
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

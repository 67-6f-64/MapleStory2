#ifndef MainApp_h__
#define MainApp_h__

#include "TimeMgr.h"

class CMainApp
{
public:
	CMainApp();
	~CMainApp();

public:
	int InitApp(void);
	void Update(void);

public:
	static CMainApp* Create(void);

private:
	void Release(void);

	int SearchEmtySocketArray(void);
	int ClientCount(void);
	void Connected(SOCKET* pSocket, int iIndex);
	void DisConnectedCheck(int iSize, int iIndex);

	void RecvData(NETDATA* pData, int iIndex);

	///////////////////////////////////////////////////////////////
	void PlayerUpdate(NETDATA* pData, int iIndex);
	void PlayerCreate(NETDATA* pData, int iIndex);

	void BulletCreate(NETDATA* pData, int iIndex);

	void SceneChangeAll(NETDATA* pData);
	void SceneCheck(NETDATA* pData, int iIndex);

	void SendChatting(NETDATA* pData, int iIndex);

	/*void SendFallCube(NETDATA* pData);*/

	void SendFallItem(NETDATA* pData, int iIndex);
	///////////////////////////////////////////////////////////////

	// 전 클라이언트에게 전송
	void SendAll(NETDATA* pData);
	// 메세지를 송신한 클라이언트에게만 전송
	void SendOne(NETDATA* pData, int iIndex);
	// 메세지를 송신한 클라이언트를 제외하고 모두 전송
	void SendOther(NETDATA* pData, int iIndex);

	///////////////////////////////////////////////////////////////

	void ClientSocketThread(void);

	// 소켓연결 에러 체크
	// 연결이 끊어지면 false 반환, 연결이 되어있으면 true반환
	bool GetConnectCheck(int iSize, int iIndex);

	///////////////////////////////////////////////////////////////

	// FPS
	void FPSCheck(void);

private:
	WORD		m_wSceneType;				// 서버가 가지고있는 씬 타입
	int			m_iClient;					// 설정된 이벤트 핸들 갯수
	int			m_iClientID;				// 클라이언트 ID
	u_short		m_PORT;						// 사용할 포트번호
	SOCKET		m_socket_array[MAX_SOCKET];	// 다중 접속을 위한 소켓 배열
	HANDLE		m_hEvent_array[MAX_SOCKET];

	SOCKADDR_IN	m_servAddr;					// 대기용 소켓 주소 정보

private:
	CTimeMgr	m_TimeMgr;
	int			m_iFps;
	float		m_fTime;
};

#endif
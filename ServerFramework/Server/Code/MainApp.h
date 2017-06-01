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

	// �� Ŭ���̾�Ʈ���� ����
	void SendAll(NETDATA* pData);
	// �޼����� �۽��� Ŭ���̾�Ʈ���Ը� ����
	void SendOne(NETDATA* pData, int iIndex);
	// �޼����� �۽��� Ŭ���̾�Ʈ�� �����ϰ� ��� ����
	void SendOther(NETDATA* pData, int iIndex);

	///////////////////////////////////////////////////////////////

	void ClientSocketThread(void);

	// ���Ͽ��� ���� üũ
	// ������ �������� false ��ȯ, ������ �Ǿ������� true��ȯ
	bool GetConnectCheck(int iSize, int iIndex);

	///////////////////////////////////////////////////////////////

	// FPS
	void FPSCheck(void);

private:
	WORD		m_wSceneType;				// ������ �������ִ� �� Ÿ��
	int			m_iClient;					// ������ �̺�Ʈ �ڵ� ����
	int			m_iClientID;				// Ŭ���̾�Ʈ ID
	u_short		m_PORT;						// ����� ��Ʈ��ȣ
	SOCKET		m_socket_array[MAX_SOCKET];	// ���� ������ ���� ���� �迭
	HANDLE		m_hEvent_array[MAX_SOCKET];

	SOCKADDR_IN	m_servAddr;					// ���� ���� �ּ� ����

private:
	CTimeMgr	m_TimeMgr;
	int			m_iFps;
	float		m_fTime;
};

#endif
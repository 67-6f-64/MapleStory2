#ifndef Network_h__
#define Network_h__

#include "Engine_Macro.h"
#include "Include.h"

#ifndef NETMGR
#define NETMGR CNetwork::GetInstance()
#endif // !NETMGR

namespace Engine
{
	class CLayer;
}

typedef map<WORD, Engine::CLayer*>* LPMAPLAYER;

class CNetwork
{
	DECLARE_SINGLETON(CNetwork)
	
private:
	CNetwork();

public:
	~CNetwork();

public:
	HRESULT InitNetwork(void);

	void SendMsg(NETDATA tData);
	NETDATA RecvMsg(void);
	void Release(void);

public:
	void SetDevice(LPDIRECT3DDEVICE9 pDevice);

	void RecvData(void);
	void SetLayer(LPMAPLAYER pLayer);

	// �÷��̾� �Լ�
	void PlayerCreate(NETDATA tData);
	void PlayerDelete(int iIndex);
	void PlayerUpdate(NETDATA tData);

	// �� ���� �÷��� ���ÿ� �Լ�
	// ���� ���� �÷��׿� �ٲ��� �� Ÿ���� ���� �˷��ֱ�����
	void SetSceneChange(bool* pB) { m_pbSceneChange = pB; }
	void SetSceneType(WORD* pType) { m_pwSceneType = pType; }
	void SceneChange(WORD wType);

	// ä�� �Լ�
	void Chat(NETDATA tData);

	// PartyFall ť���ε��� ���� �Լ�
	void SetFallCube(NETDATA tData);

	// ���� �÷��̾� ���Ӽ��� �˷��ִ� �Լ�
	int GetPlayerSize(void) { return m_iPlayerSize; }

private:
	// ���� ����
	SOCKET socket_client;

	// ������ ���̾ �ٲ�Ƿ� �����͸� �������־����		
	LPMAPLAYER			m_pMapLayer;
	LPDIRECT3DDEVICE9	m_pDevice;

	// �� ������ ���� ������
	bool*				m_pbSceneChange;
	WORD*				m_pwSceneType;

	int					m_iPlayerSize;
};



#endif // !Network_h__
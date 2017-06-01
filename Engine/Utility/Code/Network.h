#ifndef Network_h__
#define Network_h__

#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "Engine_Include.h"


BEGIN(Engine)

class CLayer;
class ENGINE_DLL CNetwork
{
	typedef std::map<WORD, CLayer*>* LPMAPLAYER;
	typedef std::map<WORD, void*> MAPDATA;
	typedef std::map<WORD, NETDATA*> MAPPLAYERDATA;

public:
	CNetwork();
	virtual ~CNetwork();

public:
	HRESULT InitNetwork(void);

	void SendMsg(NETDATA tData);
	NETDATA RecvMsg(void);
	void Release(void);

public:
	void SetDevice(LPDIRECT3DDEVICE9 pDevice);	
	void SetLayer(LPMAPLAYER pLayer);

	// -- ��ӹ��� ��ü�� �������ؾ��ϴ� �Լ���
	///////////////////////////////////////////////////////////////
	virtual void RecvData(void)PURE;
	////////////////////////////////////////////////////////////////

	// �� ���� �÷��� ���ÿ� �Լ�
	// ���� ���� �÷��׿� �ٲ��� �� Ÿ���� ���� �˷��ֱ�����
	void SetSceneChange(bool* pB) { m_pbSceneChange = pB; }
	void SetSceneFlagArr(bool** ppB) { m_ppbSceneFlagArr = ppB; }
	void SetSceneFlagArrNull(void) 
	{ 
		m_ppbSceneFlagArr= &m_pNull; 
		//cout << "m_ppbSceneFlagArr �� �ʱ�ȭ: " << m_ppbSceneFlagArr << endl;
	}

	void SetSceneType(WORD* pType) { m_pwSceneType = pType; }
	void SceneChange(WORD wType);
	
	// ���� �÷��̾� ���Ӽ��� �˷��ִ� �Լ�
	int GetPlayerSize(void) { return m_iPlayerSize; }

	const void* GetMapNetData(WORD wKey);
	// Ű���� �����ϴ� �����Ͱ� ������ ������, 
	// Ű���� �����ϴ� �����Ͱ� ������ �޸� �Ҵ��ؼ� ������ ����
	void SetNetDataBuffer(WORD wKey, NETDATA tData);

protected:
	// ���� ����
	SOCKET socket_client;

	// ������ ���̾ �ٲ�Ƿ� �����͸� �������־����		
	LPMAPLAYER			m_pMapLayer;
	LPDIRECT3DDEVICE9	m_pDevice;

	// �� ������ ���� ������
	bool*				m_pbSceneChange;
	WORD*				m_pwSceneType;
	
	int					m_iPlayerSize;

	bool**				m_ppbSceneFlagArr;
	bool*				m_pNull;

	MAPDATA				m_mapNetData;

	// �÷��̾� ������Ʈ�� ������
	MAPPLAYERDATA		m_mapPlayerData;
};

END

#endif // !Network_h__
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

	// 플레이어 함수
	void PlayerCreate(NETDATA tData);
	void PlayerDelete(int iIndex);
	void PlayerUpdate(NETDATA tData);

	// 씬 변경 플레그 세팅용 함수
	// 현재 씬의 플레그와 바꿔줄 씬 타입을 씬에 알려주기위함
	void SetSceneChange(bool* pB) { m_pbSceneChange = pB; }
	void SetSceneType(WORD* pType) { m_pwSceneType = pType; }
	void SceneChange(WORD wType);

	// 채팅 함수
	void Chat(NETDATA tData);

	// PartyFall 큐브인덱스 세팅 함수
	void SetFallCube(NETDATA tData);

	// 현재 플레이어 접속수를 알려주는 함수
	int GetPlayerSize(void) { return m_iPlayerSize; }

private:
	// 소켓 생성
	SOCKET socket_client;

	// 씬마다 레이어가 바뀌므로 포인터를 가지고있어야함		
	LPMAPLAYER			m_pMapLayer;
	LPDIRECT3DDEVICE9	m_pDevice;

	// 씬 변경을 위한 포인터
	bool*				m_pbSceneChange;
	WORD*				m_pwSceneType;

	int					m_iPlayerSize;
};



#endif // !Network_h__
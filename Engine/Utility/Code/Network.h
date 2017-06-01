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

	// -- 상속받은 객체가 재정의해야하는 함수들
	///////////////////////////////////////////////////////////////
	virtual void RecvData(void)PURE;
	////////////////////////////////////////////////////////////////

	// 씬 변경 플레그 세팅용 함수
	// 현재 씬의 플레그와 바꿔줄 씬 타입을 씬에 알려주기위함
	void SetSceneChange(bool* pB) { m_pbSceneChange = pB; }
	void SetSceneFlagArr(bool** ppB) { m_ppbSceneFlagArr = ppB; }
	void SetSceneFlagArrNull(void) 
	{ 
		m_ppbSceneFlagArr= &m_pNull; 
		//cout << "m_ppbSceneFlagArr 널 초기화: " << m_ppbSceneFlagArr << endl;
	}

	void SetSceneType(WORD* pType) { m_pwSceneType = pType; }
	void SceneChange(WORD wType);
	
	// 현재 플레이어 접속수를 알려주는 함수
	int GetPlayerSize(void) { return m_iPlayerSize; }

	const void* GetMapNetData(WORD wKey);
	// 키값에 대응하는 데이터가 있으면 값변경, 
	// 키값에 대응하는 데이터가 없으면 메모리 할당해서 데이터 세팅
	void SetNetDataBuffer(WORD wKey, NETDATA tData);

protected:
	// 소켓 생성
	SOCKET socket_client;

	// 씬마다 레이어가 바뀌므로 포인터를 가지고있어야함		
	LPMAPLAYER			m_pMapLayer;
	LPDIRECT3DDEVICE9	m_pDevice;

	// 씬 변경을 위한 포인터
	bool*				m_pbSceneChange;
	WORD*				m_pwSceneType;
	
	int					m_iPlayerSize;

	bool**				m_ppbSceneFlagArr;
	bool*				m_pNull;

	MAPDATA				m_mapNetData;

	// 플레이어 업데이트용 데이터
	MAPPLAYERDATA		m_mapPlayerData;
};

END

#endif // !Network_h__
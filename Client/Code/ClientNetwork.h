#ifndef ClientNetwork_h__
#define ClientNetwork_h__

#include "Network.h"
#include "Include.h"

#ifndef NETMGR
#define NETMGR	CClientNetwork::GetInstance()
#endif // !NETMGR

class CClientNetwork
	: public Engine::CNetwork
{
	DECLARE_SINGLETON(CClientNetwork)

public:
	CClientNetwork();
	virtual ~CClientNetwork();

public:
	// -- 상속받은 객체가 재정의해야하는 함수들
	///////////////////////////////////////////////////////////////
	virtual void RecvData(void);

	// 플레이어 함수
	void PlayerCreate(Engine::NETDATA tData);
	void PlayerDelete(int iIndex);
	void PlayerUpdate(Engine::NETDATA tData);

	// 채팅 함수
	void Chat(Engine::NETDATA tData);

	// PartyFall 아이템 세팅 함수
	void SetFallItem(Engine::NETDATA tData);

	void CreateBullet(Engine::NETDATA tData);
	////////////////////////////////////////////////////////////////

	void ErrorCheck(void);

private:
	bool	m_bClientSetting;
};

#endif
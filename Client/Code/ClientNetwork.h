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
	// -- ��ӹ��� ��ü�� �������ؾ��ϴ� �Լ���
	///////////////////////////////////////////////////////////////
	virtual void RecvData(void);

	// �÷��̾� �Լ�
	void PlayerCreate(Engine::NETDATA tData);
	void PlayerDelete(int iIndex);
	void PlayerUpdate(Engine::NETDATA tData);

	// ä�� �Լ�
	void Chat(Engine::NETDATA tData);

	// PartyFall ������ ���� �Լ�
	void SetFallItem(Engine::NETDATA tData);

	void CreateBullet(Engine::NETDATA tData);
	////////////////////////////////////////////////////////////////

	void ErrorCheck(void);

private:
	bool	m_bClientSetting;
};

#endif
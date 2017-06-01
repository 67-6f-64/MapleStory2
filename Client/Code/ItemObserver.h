
#ifndef ItemObserver_h__
#define ItemObserver_h__

#include "Observer.h"
#include "Include.h"

class CPartyFallItem;
class CClientNetwork;
class CItemObserver
	: public Engine::CObserver
{
private:
	CItemObserver(void);

public:
	virtual ~CItemObserver(void);

public:
	virtual void Update(int message);

	bool CollideCheck(PLAYER_DATA* PlayerData);

public:
	static CItemObserver* Create(void);

private:
	vector<CPartyFallItem*> m_vecItem;
	CClientNetwork*				m_pNet;
};

#endif // ItemObserver_h__
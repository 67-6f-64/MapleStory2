
#ifndef Battle_ItemObserver_h__
#define Battle_ItemObserver_h__

#include "Observer.h"
#include "Include.h"

class CItem;
class CBattle_ItemObserver
	: public Engine::CObserver
{
private:
	CBattle_ItemObserver(void);

public:
	virtual ~CBattle_ItemObserver(void);

public:
	virtual void Update(int message);

	bool CollideCheck(BATTLE_PLAYER_DATA* PlayerData);

public:
	static CBattle_ItemObserver* Create(void);

private:
	vector<CItem*> m_vecItem;
};

#endif // Battle_ItemObserver_h__
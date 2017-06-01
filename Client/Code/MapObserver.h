
#ifndef MapObserver_h__
#define MapObserver_h__

#include "Observer.h"
#include "Include.h"

class CMeteor;
class CMapObserver
	: public Engine::CObserver
{
private:
	CMapObserver(void);

public:
	virtual ~CMapObserver(void);

public:
	virtual void Update(int message);

	bool MeteorCollideCheck(BATTLE_PLAYER_DATA* PlayerData);

public:
	static CMapObserver* Create(void);

private:
	list<CMeteor*> m_listMeteor;
};

#endif // MapObserver_h__
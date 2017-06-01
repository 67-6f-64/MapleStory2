
#ifndef PortalObserver_h__
#define PortalObserver_h__

#include "Observer.h"
#include "Include.h"

class CPortal;
class CPortalObserver
	: public Engine::CObserver
{
private:
	CPortalObserver(void);

public:
	virtual ~CPortalObserver(void);

public:
	virtual void Update(int message);

	bool CollideCheck(D3DXVECTOR3 PlayerPos);

	SCENEID GetSceneId(void) { return m_eSceneID; }

public:
	static CPortalObserver* Create(void);

private:
	vector<CPortal*> m_vecPortal;

	SCENEID			 m_eSceneID;
};

#endif // PortalObserver_h__
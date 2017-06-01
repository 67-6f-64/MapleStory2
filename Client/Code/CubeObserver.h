
#ifndef CubeObserver_h__
#define CubeObserver_h__

#include "Observer.h"

class CPartyFallCube;
class CClientNetwork;
class CCubeObserver
	: public Engine::CObserver
{
private:
	CCubeObserver(void);

public:
	virtual ~CCubeObserver(void);

public:
	virtual void Update(int message);
	bool CollideCheck(D3DXVECTOR3* vecPos);

public:
	static CCubeObserver* Create(void);

private:
	vector<CPartyFallCube*> m_vecPartyFallCube;
	int						m_iIndex;

	CClientNetwork*				m_pNet;
};

#endif // CubeObserver_h__
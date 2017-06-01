/*!
* \file CollisionMgr.h
* \date 2017/05/11 16:35
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"

namespace Engine
{
	class CCollision;
}

class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr)

public:
	enum COLLISIONID { COL_TERRAIN, COL_MOUSE };

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);

public:
	HRESULT AddColObject(COLLISIONID eCollisionID
		, const Engine::VTXTEX* pTerrainVtx = NULL);
	Engine::CCollision* CloneColObject(COLLISIONID eCollisionID);

private:
	void Release(void);

private:
	typedef map<COLLISIONID, Engine::CCollision*>		MAPCOLLISION;
	MAPCOLLISION		m_mapCollision;
};

#endif // CollisionMgr_h__
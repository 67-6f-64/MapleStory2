#include "stdafx.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "MouseCol.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr(void)
{

}

CCollisionMgr::~CCollisionMgr(void)
{
	Release();
}

HRESULT CCollisionMgr::AddColObject(COLLISIONID eCollisionID, const Engine::VTXTEX* pTerrainVtx /*= NULL*/)
{
	Engine::CCollision*		pCollision = NULL;

	MAPCOLLISION::iterator	iter = m_mapCollision.find(eCollisionID);
	if (iter != m_mapCollision.end())
		return E_FAIL;

	switch (eCollisionID)
	{
	case COL_TERRAIN:
		pCollision = CTerrainCol::Create(pTerrainVtx);
		break;

	case COL_MOUSE:
		pCollision = CMouseCol::Create(pTerrainVtx);
		break;
	}
	NULL_CHECK_RETURN(pCollision, E_FAIL);
	m_mapCollision.insert(MAPCOLLISION::value_type(eCollisionID, pCollision));
	return S_OK;
}

Engine::CCollision* CCollisionMgr::CloneColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator		iter = m_mapCollision.find(eCollisionID);
	if (iter == m_mapCollision.end())
		return NULL;

	return iter->second->Clone();
}

void CCollisionMgr::Release(void)
{
	for (MAPCOLLISION::iterator	iter = m_mapCollision.begin();
		iter != m_mapCollision.end(); ++iter)
	{
		Safe_Delete(iter->second);
	}
	m_mapCollision.clear();
}


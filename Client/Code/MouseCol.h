/*!
* \file MouseCol.h
* \date 2017/05/11 16:51
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

#ifndef MouseCol_h__
#define MouseCol_h__

#include "Collision.h"

class CMouseCol
	: public Engine::CCollision
{
private:
	CMouseCol(const Engine::VTXTEX* pTerrainVtx);

public:
	virtual ~CMouseCol(void);

public:
	virtual Engine::CCollision* Clone(void);

public:
	bool PickTerrain(D3DXVECTOR3* pOut
		, const Engine::VTXTEX* pTerrainVtx);

public:
	static CMouseCol* Create(const Engine::VTXTEX* pTerrainVtx);

private:
	void Initialize(void);
	void Release(void);

private:
	const Engine::VTXTEX*		m_pTerrainVtx;
};

#endif // MouseCol_h__
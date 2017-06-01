/*!
* \file TerrainCol.h
* \date 2017/05/11 16:36
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

#ifndef TerrainCol_h__
#define TerrainCol_h__

#include "Collision.h"

class CTerrainCol
	: public Engine::CCollision
{
private:
	CTerrainCol(const Engine::VTXTEX* pTerrainVtx);

public:
	virtual ~CTerrainCol(void);

public:
	virtual Engine::CCollision* Clone(void);

public:
	void SetColInfo(D3DXVECTOR3* pPos);

public:
	virtual void Update(void);

public:
	static CTerrainCol* Create(const Engine::VTXTEX* pTerrainVtx);

	const Engine::VTXTEX* GetTerrainVtx(void) { return m_pTerrainVtx; }

private:
	void Release(void);

private:
	D3DXVECTOR3*				m_pPos;
	const Engine::VTXTEX*		m_pTerrainVtx;

};

#endif // TerrainCol_h__
#include "stdafx.h"
#include "TerrainCol.h"

#include "Include.h"

CTerrainCol::CTerrainCol(const Engine::VTXTEX* pTerrainVtx)
	: m_pTerrainVtx(pTerrainVtx)
{

}

CTerrainCol::~CTerrainCol(void)
{
	Release();
}

Engine::CCollision* CTerrainCol::Clone(void)
{
	CCollision*		pCollision = new CTerrainCol(*this);

	++(*m_pwRefCnt);

	return pCollision;
}

void CTerrainCol::SetColInfo(D3DXVECTOR3* pPos)
{
	m_pPos = pPos;
}

void CTerrainCol::Update(void)
{
	int		iIndex = (int(m_pPos->z) / VTXITV) * VTXCNTX + (int(m_pPos->x) / VTXITV);

	float		fRatioX = (m_pPos->x - m_pTerrainVtx[iIndex + VTXCNTX].vPos.x) / VTXITV;
	float		fRatioZ = (m_pTerrainVtx[iIndex + VTXCNTX].vPos.z - m_pPos->z) / VTXITV;

	D3DXPLANE		Plane;

	if (fRatioX > fRatioZ)	//오른쪽 위
	{
		D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[iIndex + VTXCNTX].vPos
			, &m_pTerrainVtx[iIndex + VTXCNTX + 1].vPos
			, &m_pTerrainVtx[iIndex + 1].vPos);
	}
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[iIndex + VTXCNTX].vPos
			, &m_pTerrainVtx[iIndex + 1].vPos
			, &m_pTerrainVtx[iIndex].vPos);
	}
	//ax + by + cz + d = 0
	//by = -ax -cz -d
	//y = (-ax -cz -d) / b
	m_pPos->y = (-Plane.a * m_pPos->x - Plane.c * m_pPos->z - Plane.d) / Plane.b;
	m_pPos->y += 1.f;
}

CTerrainCol* CTerrainCol::Create(const Engine::VTXTEX* pTerrainVtx)
{
	NULL_CHECK_RETURN(pTerrainVtx, NULL);

	return new CTerrainCol(pTerrainVtx);
}

void CTerrainCol::Release(void)
{
	if (*m_pwRefCnt == 0)
		Engine::Safe_Delete(m_pwRefCnt);
	else
		--(*m_pwRefCnt);
}

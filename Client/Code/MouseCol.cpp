#include "stdafx.h"
#include "MouseCol.h"
#include "Export_Function.h"
#include "Include.h"

CMouseCol::CMouseCol(const Engine::VTXTEX* pTerrainVtx)
	:m_pTerrainVtx(pTerrainVtx)
{

}

CMouseCol::~CMouseCol(void)
{
	Release();
}

Engine::CCollision* CMouseCol::Clone(void)
{
	CCollision*		pCollision = new CMouseCol(*this);

	++(*m_pwRefCnt);

	return pCollision;
}

CMouseCol* CMouseCol::Create(const Engine::VTXTEX* pTerrainVtx)
{
	NULL_CHECK_RETURN(pTerrainVtx, nullptr);

	CMouseCol* pMouseCol = new CMouseCol(pTerrainVtx);
	pMouseCol->Initialize();

	return pMouseCol;
}

void CMouseCol::Initialize(void)
{
}

void CMouseCol::Release(void)
{
	if (*m_pwRefCnt == 0)
	{
		Engine::Safe_Delete(m_pwRefCnt);
	}
	else
		--(*m_pwRefCnt);
}

bool CMouseCol::PickTerrain(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx)
{
	return false;
}
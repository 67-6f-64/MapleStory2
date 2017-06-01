#include "WallTex.h"

Engine::CWallTex::CWallTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

Engine::CWallTex::~CWallTex(void)
{
	
}

Engine::CResources* Engine::CWallTex::CloneResource(void)
{
	CResources*		pResource = new CWallTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

HRESULT Engine::CWallTex::CreateBuffer(const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal)
{
	m_dwVtxSize = sizeof(VTXLIGHT);
	m_dwVtxCnt = wCntY * wCntZ;
	m_dwVtxFVF = VTXLIGHT_FVF;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	m_dwTriCnt = (wCntY - 1) * (wCntZ - 1) * 2;

	HRESULT		hr = CVIBuffer::CreateBuffer();
	FAILED_CHECK(hr);

	VTXLIGHT*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int		iIndex = 0;
	for (int z = 0; z < wCntZ; ++z)
	{
		for (int y = 0; y < wCntY; ++y)
		{
			iIndex = z * wCntY + y;

			pVtxTex[iIndex].vPos = D3DXVECTOR3(0.f, float(y) * wItv, float(z) * wItv);
			pVtxTex[iIndex].vTexUV = D3DXVECTOR2(z / (wCntZ - 1.f), y / (wCntY - 1.f));
			pVtxTex[iIndex].vNormal = vNormal;
		}
	}

	m_pVB->Unlock();

	INDEX32*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int		iTriCnt = 0;
	for (int z = 0; z < wCntZ - 1; ++z)
	{
		for (int y = 0; y < wCntY - 1; ++y)
		{
			int iIndex = z * wCntY + y;
			pIndex[iTriCnt]._1 = iIndex + wCntY;
			pIndex[iTriCnt]._2 = iIndex + wCntY + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntY;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;
			++iTriCnt;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

Engine::CWallTex* Engine::CWallTex::Create(LPDIRECT3DDEVICE9 pDevice
	, const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal)
{
	CWallTex*	pBuffer = new CWallTex(pDevice);
	if (FAILED(pBuffer->CreateBuffer(wCntX, wCntY, wCntZ, wItv, vNormal)))
	{
		Safe_Delete(pBuffer);
	}
	return pBuffer;
}

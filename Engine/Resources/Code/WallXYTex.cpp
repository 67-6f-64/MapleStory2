#include "WallXYTex.h"

Engine::CWallXYTex::CWallXYTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

Engine::CWallXYTex::~CWallXYTex(void)
{
	
}

Engine::CResources* Engine::CWallXYTex::CloneResource(void)
{
	CResources*		pResource = new CWallXYTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

HRESULT Engine::CWallXYTex::CreateBuffer(const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal)
{
	m_dwVtxSize = sizeof(VTXLIGHT);
	m_dwVtxCnt = wCntX * wCntY;
	m_dwVtxFVF = VTXLIGHT_FVF;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	m_dwTriCnt = (wCntX - 1) * (wCntY - 1) * 2;

	HRESULT		hr = CVIBuffer::CreateBuffer();
	FAILED_CHECK(hr);

	VTXLIGHT*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int		iIndex = 0;
	for (int y = 0; y < wCntY; ++y)
	{
		for (int x = 0; x < wCntX; ++x)
		{
			iIndex = y * wCntX + x;

			pVtxTex[iIndex].vPos = D3DXVECTOR3(float(x) * wItv, float(y) * wItv, 0.f);
			pVtxTex[iIndex].vTexUV = D3DXVECTOR2(x / (wCntX - 1.f), y / (wCntY - 1.f));
			pVtxTex[iIndex].vNormal = vNormal;
		}
	}

	m_pVB->Unlock();

	INDEX32*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int		iTriCnt = 0;
	for (int y = 0; y < wCntY - 1; ++y)
	{
		for (int x = 0; x < wCntX - 1; ++x)
		{
			int iIndex = y * wCntX + x;
			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;
			++iTriCnt;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

Engine::CWallXYTex* Engine::CWallXYTex::Create(LPDIRECT3DDEVICE9 pDevice
	, const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal)
{
	CWallXYTex*	pBuffer = new CWallXYTex(pDevice);
	if (FAILED(pBuffer->CreateBuffer(wCntX, wCntY, wCntZ, wItv, vNormal)))
	{
		Safe_Delete(pBuffer);
	}
	return pBuffer;
}

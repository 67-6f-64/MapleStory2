#include "RcTex.h"

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

Engine::CRcTex::~CRcTex(void)
{
	
}

Engine::CResources* Engine::CRcTex::CloneResource(void)
{
	CResources*		pResource = new CRcTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

HRESULT Engine::CRcTex::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = VTXTEX_FVF;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 2;

	HRESULT		hr = CVIBuffer::CreateBuffer();
	FAILED_CHECK(hr);

	VTXTEX*		pVtxCol = NULL;
	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	pVtxCol[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxCol[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVtxCol[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxCol[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVtxCol[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxCol[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVtxCol[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxCol[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRcTex*	pBuffer = new CRcTex(pDevice);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}
	return pBuffer;
}

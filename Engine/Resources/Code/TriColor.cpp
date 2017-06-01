#include "TriColor.h"

Engine::CTriColor::CTriColor(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

Engine::CTriColor::~CTriColor(void)
{

}

Engine::CResources* Engine::CTriColor::CloneResource(void)
{
	CResources*		pResource = new CTriColor(*this);

	++(*m_pwRefCnt);

	return pResource;
}

HRESULT Engine::CTriColor::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwVtxFVF = VTXCOL_FVF;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 1;

	HRESULT		hr = CVIBuffer::CreateBuffer();
	FAILED_CHECK(hr);

	VTXCOL*		pVtxCol = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	pVtxCol[0].vPos = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxCol[0].dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVtxCol[1].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxCol[1].dwColor = D3DCOLOR_ARGB(255, 255, 255, 0);

	pVtxCol[2].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxCol[2].dwColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	m_pVB->Unlock();
	

	return S_OK;
}

Engine::CTriColor* Engine::CTriColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriColor*	pBuffer = new CTriColor(pDevice);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}
	return pBuffer;
}

void Engine::CTriColor::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
}

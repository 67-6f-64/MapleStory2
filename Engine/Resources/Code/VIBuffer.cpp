#include "VIBuffer.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
, m_pVB(NULL)
, m_dwVtxSize(0)
, m_dwVtxCnt(0)
, m_dwVtxFVF(0)
, m_pIB(NULL)
, m_dwIdxSize(0)
, m_dwTriCnt(0)
, m_IdxFmt(D3DFMT_UNKNOWN)
{

}

Engine::CVIBuffer::~CVIBuffer(void)
{
	Release();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	HRESULT		hr = NULL;
	hr = m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	FAILED_CHECK(hr);

	hr = m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	FAILED_CHECK(hr);

	return S_OK;
}

void Engine::CVIBuffer::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void Engine::CVIBuffer::Release(void)
{
	if (*m_pwRefCnt == 0)
	{
		Safe_Release(m_pVB);
		Safe_Release(m_pIB);
		CResources::Release();
	}
	else
		--(*m_pwRefCnt);
}

void Engine::CVIBuffer::GetVtxInfo(void* pVertex)
{
	void*	pOriVretex = NULL;
	m_pVB->Lock(0, 0, &pOriVretex, 0);
	memcpy(pVertex, pOriVretex, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetVtxInfo(void* pVertex)
{
	void*	pOriVretex = NULL;
	m_pVB->Lock(0, 0, &pOriVretex, 0);
	memcpy(pOriVretex, pVertex, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}
#include "CubeTex.h"

Engine::CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pDevice)
	: CVIBuffer(pDevice)
{

}

Engine::CCubeTex::~CCubeTex(void)
{

}

Engine::CResources* Engine::CCubeTex::CloneResource(void)
{
	CResources*		pResource = new CCubeTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

HRESULT Engine::CCubeTex::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_CUBE;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 12;

	HRESULT		hr = CVIBuffer::CreateBuffer();
	FAILED_CHECK(hr);

	VTXCUBE*		pVtxCol = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	pVtxCol[0].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxCol[0].vTex = D3DXVECTOR3(-1.f, -1.f, -1.f);

	pVtxCol[1].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxCol[1].vTex = D3DXVECTOR3(-1.f, 1.f, -1.f);

	pVtxCol[2].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxCol[2].vTex = D3DXVECTOR3(1.f, 1.f, -1.f);

	pVtxCol[3].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxCol[3].vTex = D3DXVECTOR3(1.f, -1.f, -1.f);

	pVtxCol[4].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxCol[4].vTex = D3DXVECTOR3(-1.f, -1.f, 1.f);

	pVtxCol[5].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxCol[5].vTex = D3DXVECTOR3(-1.f, 1.f, 1.f);

	pVtxCol[6].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxCol[6].vTex = D3DXVECTOR3(1.f, 1.f, 1.f);

	pVtxCol[7].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxCol[7].vTex = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//// front side
	pIndex[0]._1 = 0; pIndex[0]._2 = 1; pIndex[0]._3 = 2;
	pIndex[1]._1 = 0; pIndex[1]._2 = 2; pIndex[1]._3 = 3;

	//// back side
	pIndex[2]._1 = 4; pIndex[2]._2 = 6; pIndex[2]._3 = 5;
	pIndex[3]._1 = 4; pIndex[3]._2 = 7; pIndex[3]._3 = 6;

	//// left side
	pIndex[4]._1 = 4; pIndex[4]._2 = 5; pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1; pIndex[5]._3 = 0;

	//// right side
	pIndex[6]._1 = 3; pIndex[6]._2 = 2; pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6; pIndex[7]._3 = 7;

	//// top side
	pIndex[8]._1 = 1; pIndex[8]._2 = 5; pIndex[8]._3 = 6;
	pIndex[9]._1 = 1; pIndex[9]._2 = 6; pIndex[9]._3 = 2;

	//// bottom side
	pIndex[10]._1 = 4; pIndex[10]._2 = 0; pIndex[10]._3 = 3;
	pIndex[11]._1 = 4; pIndex[11]._2 = 3; pIndex[11]._3 = 7;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CCubeTex* Engine::CCubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTex*	pBuffer = new CCubeTex(pDevice);
	if (FAILED(pBuffer->CreateBuffer()))
	{
		Safe_Delete(pBuffer);
	}
	return pBuffer;
}

#include "stdafx.h"
#include "PlayerHit.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Include.h"
#include "Frame.h"

CPlayerHit::CPlayerHit(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_bIsEnd(false)
{

}

CPlayerHit::~CPlayerHit(void)
{
	Release();
}

HRESULT CPlayerHit::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(3.f, 3.f, 3.f);

	return S_OK;
}

int CPlayerHit::Update(void)
{
	Engine::CGameObject::Update();
	
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	return 0;
}

void CPlayerHit::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTexture->Render(0, DWORD(m_pFrame->m_tFrame.fFrame));
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

CPlayerHit* CPlayerHit::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerHit*	pObj = new CPlayerHit(pDevice);

	if (FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}


bool CPlayerHit::GetHitFrame(void)
{
	if (m_pFrame->m_tFrame.fFrame >= m_pFrame->m_tFrame.fMax - 1.f)
	{
		m_bIsEnd = true;
	}
	else
		m_bIsEnd = false;

	return m_bIsEnd;
}

HRESULT CPlayerHit::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"PlayerHit");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));


	// Frame
	Engine::FRAME tFrame = Engine::FRAME(0.f, 12.f, 3.f);
	pCompontent = m_pFrame = Engine::CFrame::Create(tFrame);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	return S_OK;
}

void CPlayerHit::Release(void)
{

}

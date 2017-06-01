#include "stdafx.h"
#include "MainUI.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

CMainUI::CMainUI(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pTexture(NULL)
{
}

CMainUI::~CMainUI(void)
{
	Release();
}

HRESULT CMainUI::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(WINCX / 2.f, WINCY / 2.f, 1.f);
	m_pInfo->m_vPos = D3DXVECTOR3(WINCX / 2.f, WINCY / 2.f, 0.f);

	m_pInfo->m_vPos.y = -m_pInfo->m_vPos.y;
	m_pInfo->m_vPos.x -= WINCX / 2.f;
	m_pInfo->m_vPos.y += WINCY / 2.f;

	D3DXMatrixOrthoLH(&m_matOrthoProj, WINCX, WINCY, 0.0f, 1.0f);
	D3DXMatrixIdentity(&m_matIdentity);
	
	return S_OK;
}

int CMainUI::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CMainUI::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000001);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 원래 값 저장
	D3DXMATRIX matView2, matProj2;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView2);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj2);

	// 직교 투영
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matIdentity);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matOrthoProj);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();

	// 원복
	m_pDevice->SetTransform(D3DTS_VIEW, &matView2);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj2);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CMainUI* CMainUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMainUI*		pLogo = new CMainUI(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CMainUI::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_MainUI");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CMainUI::Release(void)
{

}

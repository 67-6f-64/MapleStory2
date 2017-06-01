#include "stdafx.h"
#include "MeteorEffect.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Include.h"
#include "Frame.h"

CMeteorEffect::CMeteorEffect(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pFrame(nullptr)
{

}

CMeteorEffect::~CMeteorEffect(void)
{
	Release();
}

HRESULT CMeteorEffect::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(5.f, 5.f, 5.f);

	return S_OK;
}

int CMeteorEffect::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	Engine::CGameObject::Update();

	if (DWORD(m_pFrame->m_tFrame.fFrame) == 15)
		SetDead();

	return 0;
}

void CMeteorEffect::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTexture->Render(0, DWORD(m_pFrame->m_tFrame.fFrame));
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CMeteorEffect* CMeteorEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeteorEffect*		pLogo = new CMeteorEffect(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}
HRESULT CMeteorEffect::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// Transform
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	// Frame
	pCompontent = m_pFrame = Engine::CFrame::Create(Engine::FRAME(0.f, 16.f, 16.f));
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTexXZ");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Meteor_Attack");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CMeteorEffect::Release(void)
{

}

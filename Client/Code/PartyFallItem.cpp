#include "stdafx.h"
#include "PartyFallItem.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"
#include "Frustum.h"

CPartyFallItem::CPartyFallItem(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_bCollide(false)
{

}

CPartyFallItem::~CPartyFallItem(void)
{
	Release();
}

HRESULT CPartyFallItem::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CPartyFallItem::Update(void)
{
	// 빙그르 돔
	m_pInfo->m_fAnlge[Engine::ANGLE_Y] += 0.01f;

	Engine::CGameObject::Update();

	// 최초 낙하
	Fall();

	return 0;
}

void CPartyFallItem::Render(void)
{
	if (m_bCollide) // 충돌했으면 라운드가 다시 시작되기 전까진 렌더하지 않는다.
		return;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();
}

CPartyFallItem* CPartyFallItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyFallItem*		pLogo = new CPartyFallItem(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

void CPartyFallItem::Fall()
{
	if (m_pInfo->m_vPos.y > 0.f)
	{
		m_pInfo->m_vPos.y -= 3 * Engine::Get_TimeMgr()->GetTime();
	}

	if (m_pInfo->m_vPos.y < 0.f)
		m_pInfo->m_vPos.y = 0.f;
}

HRESULT CPartyFallItem::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_CubeItem");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	return S_OK;
}

void CPartyFallItem::Release(void)
{
}

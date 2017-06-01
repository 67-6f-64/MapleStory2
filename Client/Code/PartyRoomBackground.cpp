#include "stdafx.h"
#include "PartyRoomBackground.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

CPartyRoomBackground::CPartyRoomBackground(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pTexture(NULL)
{
}

CPartyRoomBackground::~CPartyRoomBackground(void)
{
	Release();
}

HRESULT CPartyRoomBackground::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(468.f, 488.f, 1.f);
	m_pInfo->m_vPos.y = -100.f;
	m_pInfo->m_vPos.z = 400.f;

	return S_OK;
}

int CPartyRoomBackground::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CPartyRoomBackground::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();
}

CPartyRoomBackground* CPartyRoomBackground::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyRoomBackground*		pLogo = new CPartyRoomBackground(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CPartyRoomBackground::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Background_PartyRoom");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CPartyRoomBackground::Release(void)
{

}

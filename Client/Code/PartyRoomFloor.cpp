#include "stdafx.h"
#include "PartyRoomFloor.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

#include "Frustum.h"

CPartyRoomFloor::CPartyRoomFloor(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
{

}

CPartyRoomFloor::~CPartyRoomFloor(void)
{
	Release();
}

HRESULT CPartyRoomFloor::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);



	return S_OK;
}

int CPartyRoomFloor::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();

	return 0;
}

void CPartyRoomFloor::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();
}

CPartyRoomFloor* CPartyRoomFloor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyRoomFloor*		pLogo = new CPartyRoomFloor(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CPartyRoomFloor::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_PartyRoomFloor");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CPartyRoomFloor::Release(void)
{
}

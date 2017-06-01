#include "stdafx.h"
#include "Floor.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

#include "Frustum.h"

CFloor::CFloor(LPDIRECT3DDEVICE9 pDevice, WORD wTexture/* = 0*/)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_wTexture(wTexture)
{

}

CFloor::~CFloor(void)
{
	Release();
}

HRESULT CFloor::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	//////// ÀçÁú //////////
	D3DMATERIAL9 mtrl;

	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.f;

	m_pDevice->SetMaterial(&mtrl);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00606060);

	return S_OK;
}

int CFloor::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);

	Engine::CGameObject::Update();

	return 0;
}

void CFloor::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, m_wTexture);
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CFloor* CFloor::Create(LPDIRECT3DDEVICE9 pDevice, WORD wTexture/* = 0*/)
{
	CFloor*		pLogo = new CFloor(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CFloor::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Floor");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CFloor::Release(void)
{
}

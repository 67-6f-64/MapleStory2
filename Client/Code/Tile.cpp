#include "stdafx.h"
#include "Tile.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

CTile::CTile(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{

}

CTile::~CTile(void)
{
	Release();
}

HRESULT CTile::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(3.f, 3.f, 3.f);

	return S_OK;
}

int CTile::Update(void)
{
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	Engine::CGameObject::Update();

	return 0;
}

void CTile::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���� �׽�Ʈ
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CTile* CTile::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTile*		pLogo = new CTile(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CTile::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTexXZ");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_BastionTile");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CTile::Release(void)
{

}

#include "stdafx.h"
#include "Item.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Include.h"

CItem::CItem(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_eItemId(ITEM_END)
{

}

CItem::~CItem(void)
{
	Release();
}

HRESULT CItem::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CItem::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	Engine::CGameObject::Update();

	Fall();

	// 빌보드
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	return 0;
}

void CItem::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTexture->Render(0, DWORD(m_eItemId)); // 아이템 타입에 따라서 렌더
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItem*		pLogo = new CItem(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

void CItem::Fall(void)
{
	if (m_pInfo->m_vPos.y > 0.f)
	{
		m_pInfo->m_vPos.y -= 3 * Engine::Get_TimeMgr()->GetTime();
	}

	if (m_pInfo->m_vPos.y < 1.f)
		m_pInfo->m_vPos.y = 1.f;
}

HRESULT CItem::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// Transform
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Portion");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CItem::Release(void)
{

}

#include "stdafx.h"
#include "Portal.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Include.h"
#include "Frame.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pUITexture(nullptr)
	, m_pTexture(nullptr)
{

}

CPortal::~CPortal(void)
{
	Release();
}

HRESULT CPortal::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CPortal::Update(void)
{
	Engine::CGameObject::Update();

	// ºôº¸µå
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, nullptr, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	return 0;
}

void CPortal::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// ¾ËÆÄ Å×½ºÆ®
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Æ÷Å»
	m_pTexture->Render(0, DWORD(m_pFrame->m_tFrame.fFrame));
	m_pBuffer->Render();

	// UI
	m_pInfo->m_matWorld._42 += 1.5f;
	m_pInfo->m_matWorld._11 = 0.5f;
	m_pInfo->m_matWorld._22 = 0.5f;
	m_pInfo->m_matWorld._33 = 0.5f;
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pUITexture->Render(0, DWORD(m_pUIFrame->m_tFrame.fFrame));
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pDevice, ePORTALID eId)
{
	CPortal*		pPortal = new CPortal(pDevice);
	pPortal->SetPortalId(eId);
	if (FAILED(pPortal->Initialize()))
	{
		Engine::Safe_Delete(pPortal);
	}

	return pPortal;
}

HRESULT CPortal::AddComponent(void)
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

	// Frame
	Engine::FRAME tFrame;
	TCHAR szPortal[64];

	switch (m_ePortalId)
	{
	case PORTAL_PARTYROOM_TO_PARTYFALL:
		tFrame = Engine::FRAME(0.f, 40.f, 40.f);
		lstrcpy(szPortal, L"PartyRoom_Portal_To_PartyFall");
		break;

	case PORTAL_PARTYROOM_TO_PARTYMAZE:
		tFrame = Engine::FRAME(0.f, 8.f, 8.f);
		lstrcpy(szPortal, L"PartyRoom_Portal_To_PartyMaze");
		break;

	case PORTAL_PARTYROOM_TO_BATTLEGROUND:
		tFrame = Engine::FRAME(0.f, 14.f, 14.f);
		lstrcpy(szPortal, L"PartyRoom_Portal_To_BattleGround");
		break;
	}

	// Æ÷Å»
	pCompontent = m_pFrame = Engine::CFrame::Create(tFrame);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, szPortal);
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	// UI
	pCompontent = m_pUIFrame = Engine::CFrame::Create(Engine::FRAME(0.f, 6.f, 6.f));
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"UIFrame", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Portal_UI");
	m_pUITexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture2", pCompontent));

	return S_OK;
}

void CPortal::Release(void)
{

}

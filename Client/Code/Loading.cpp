#include "stdafx.h"
#include "Loading.h"

#include "Include.h"
#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "Frame.h"

#include "GameStart.h"

int CLoading::iCount = 0;

CLoading::CLoading(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(NULL)
	, m_pTexture(nullptr)
	, m_bCount(false)
{

}

CLoading::~CLoading(void)
{
	Release();
}

HRESULT CLoading::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CLoading::Update(void)
{
	if (m_pFrame->m_tFrame.fFrame == 0.f && !m_bCount)
	{
		m_bCount = true;
		iCount++;
	}

	if (m_bCount && m_pFrame->m_tFrame.fFrame > 0.f)
		m_bCount = false;

	Engine::CGameObject::Update();

	return 0;
}

void CLoading::Render(void)
{
	if (iCount < 4)
	{
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

		// 알파 테스트
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pTexture->Render(0, (DWORD)m_pFrame->m_tFrame.fFrame);
		m_pBuffer->Render();

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLoading*		pLoading = new CLoading(pDevice);
	if (FAILED(pLoading->Initialize()))
	{
		Engine::Safe_Delete(pLoading);
	}

	return pLoading;
}

HRESULT CLoading::AddComponent(void)
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

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_DYNAMIC, L"Loading_UI");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Loading", pCompontent));

	Engine::FRAME tFrame = Engine::FRAME(0.f, 7.f, 11.f);
	pCompontent = m_pFrame = Engine::CFrame::Create(tFrame);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	return S_OK;
}

void CLoading::Release(void)
{
}

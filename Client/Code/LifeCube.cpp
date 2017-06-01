#include "stdafx.h"
#include "LifeCube.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "TimeMgr.h"
#include "Texture.h"


CLifeCube::CLifeCube(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{
}


CLifeCube::~CLifeCube()
{
	Release();
}

int CLifeCube::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	// 라이프 회전
	m_pInfo->m_fAnlge[Engine::ANGLE_Y] += Engine::Get_TimeMgr()->GetTime() * 10.f;

	Engine::CGameObject::Update();

	return 0;
}

void CLifeCube::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 28);
	m_pBuffer->Render();
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CLifeCube * CLifeCube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLifeCube*	pObj = new CLifeCube(pDevice);

	if (FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CLifeCube::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_fAnlge[Engine::ANGLE_Z] = D3DXToRadian(5.f);

	return S_OK;
}

HRESULT CLifeCube::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// 월드 행렬
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	// 크기 20%로 세팅
	m_pInfo->m_vScale *= 0.2f;

	// 버퍼
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_CubeTex", pCompontent));

	// 텍스쳐
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Cube", pCompontent));

	return S_OK;
}

void CLifeCube::Release(void)
{
}

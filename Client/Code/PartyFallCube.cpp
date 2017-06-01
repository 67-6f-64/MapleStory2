#include "stdafx.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"
#include "Frustum.h"

CPartyFallCube::CPartyFallCube(LPDIRECT3DDEVICE9 pDevice, int iTexture)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_iTexture(iTexture)
	, m_bCollide(false)
	, m_fTime(5.f)
	, m_bShakeLeft(true)
	, m_bFall(false)
	, m_fShakeSpeed(1.f)
{

}

CPartyFallCube::~CPartyFallCube(void)
{
	Release();
}

HRESULT CPartyFallCube::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CPartyFallCube::Update(void)
{
	Collide();
	Fall();

	Engine::CGameObject::Update();

	return 0;
}

void CPartyFallCube::Render(void)
{
	if (CFrustumMgr::GetInstance()->IsSphereInFrustum(m_pInfo->m_vPos, 1) == FALSE) // 절두체 컬링
		return;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, m_iTexture);
	m_pBuffer->Render();
}

void CPartyFallCube::RollBackPos(void)
{
	m_pInfo->m_vPos = m_vecInitialPos;
}

CPartyFallCube* CPartyFallCube::Create(LPDIRECT3DDEVICE9 pDevice, int iTexture)
{
	CPartyFallCube*		pLogo = new CPartyFallCube(pDevice, iTexture);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}
	

	return pLogo;
}

void CPartyFallCube::Collide(void)
{
	if (!m_bCollide)
		return;

	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_fTime -= fTime;
	if (m_fTime < 0.f) // 5초가 지나면 낙하
		m_bFall = true;

	// 점점 더 빠르게 흔들린다
	m_fShakeSpeed += 1.5f * fTime;

	m_pInfo->m_fAnlge[Engine::ANGLE_Z] += (m_bShakeLeft) ? fTime * m_fShakeSpeed : -fTime * m_fShakeSpeed;
	
	// 좌,우로 흔들리기
	if (m_pInfo->m_fAnlge[Engine::ANGLE_Z] > 0.2f)
		m_bShakeLeft = false;
	else if(m_pInfo->m_fAnlge[Engine::ANGLE_Z] < -0.2f)
		m_bShakeLeft = true;
}

void CPartyFallCube::Fall(void)
{
	if (!m_bFall)
		return;

	m_pInfo->m_vPos.y -= 50 * Engine::Get_TimeMgr()->GetTime();
}

HRESULT CPartyFallCube::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	return S_OK;
}

void CPartyFallCube::Release(void)
{
}

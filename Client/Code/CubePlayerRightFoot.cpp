#include "stdafx.h"
#include "CubePlayerRightFoot.h"

#include "Include.h"
#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Frustum.h"


CCubePlayerRightFoot::CCubePlayerRightFoot(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(NULL)
	, m_pPlayerTransform(NULL)
	, m_fAngle(30.f)
	, m_bIsUp(true)
	, m_ePlayerState(NULL)
{

}

CCubePlayerRightFoot::~CCubePlayerRightFoot(void)
{
	Release();
}

HRESULT CCubePlayerRightFoot::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(0.2f, 0.2f, 0.5f);

	return S_OK;
}

int CCubePlayerRightFoot::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);

	FootPosition();

	if (*m_ePlayerState != PLAYER_STAND)
	{
		LookLikeWalking();
	}
	else
	{		// 가만히 있을때 발 초기화
		m_fAngle = 30.f;
		m_bIsUp = true;
		m_pInfo->m_fAnlge[Engine::ANGLE_X] = D3DXToRadian(0.f);
	}

	Engine::CGameObject::Update();

	return 0;
}

void CCubePlayerRightFoot::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();
}

CCubePlayerRightFoot* CCubePlayerRightFoot::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pPlayerInfo)
{
	CCubePlayerRightFoot*		pCubePlayerLeftFoot = new CCubePlayerRightFoot(pDevice);
	if (FAILED(pCubePlayerLeftFoot->Initialize()))
	{
		Engine::Safe_Delete(pCubePlayerLeftFoot);
	}

	pCubePlayerLeftFoot->SetPlayerTrans(pPlayerInfo);

	return pCubePlayerLeftFoot;
}

HRESULT CCubePlayerRightFoot::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;
	// Transform
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Player_Foot");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	return S_OK;
}

void CCubePlayerRightFoot::Release(void)
{

}

void CCubePlayerRightFoot::LookLikeWalking(void)
{
	if (m_bIsUp)
		m_fAngle += 0.5f;
	else
		m_fAngle -= 0.5f;

	if (m_fAngle >= 30.f)
		m_bIsUp = false;
	if (m_fAngle < 0.f)
		m_bIsUp = true;

	m_pInfo->m_fAnlge[Engine::ANGLE_X] = D3DXToRadian(m_fAngle);
}

void CCubePlayerRightFoot::FootPosition(void)
{
	m_pInfo->m_vPos = m_pPlayerTransform->m_vPos;

	m_pInfo->m_vPos.y -= 1.2f;

	D3DXVECTOR3 vecMove;
	D3DXVec3Cross(&vecMove, &m_pPlayerTransform->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pInfo->m_vPos += vecMove / 2;

	m_pInfo->m_fAnlge[Engine::ANGLE_Y] = m_pPlayerTransform->m_fAnlge[Engine::ANGLE_Y];
}

void CCubePlayerRightFoot::SetPlayerTrans(const Engine::CTransform* pPlayerInfo)
{
	m_pPlayerTransform = pPlayerInfo;
}

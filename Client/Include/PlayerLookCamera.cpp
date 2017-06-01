#include "stdafx.h"
#include "PlayerLookCamera.h"
#include "Export_Function.h"
#include "value.h"
#include "KeyMgr.h"
#include "Transform.h"

CPlayerLookCamera::CPlayerLookCamera(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice)
	, m_vDir(g_Look)
{
}

CPlayerLookCamera::~CPlayerLookCamera(void)
{
	Release();
}

HRESULT CPlayerLookCamera::Initialize(void)
{
	SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	return S_OK;
}

int CPlayerLookCamera::Update(void)
{
	D3DXMATRIX	matRotY, matView;

	D3DXVECTOR3 vecPlayer = m_pTargetInfo->m_vPos;

	// 플레이어를 따라다니는 눈
	m_vEye = D3DXVECTOR3(20.f, 7.f, 0.f) + vecPlayer;

	// 플레이어를 바라본다.
	m_vAt = vecPlayer;

	//// 플레이어 기준으로 공전하는 카메라
	float fAngle = m_pTargetInfo->m_fAnlge[Engine::ANGLE_Y] + D3DXToRadian(90.f);
	D3DXMatrixRotationY(&matRotY, fAngle);

	m_vEye -= vecPlayer;
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRotY);
	m_vEye += vecPlayer;

	SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);

	return 0;
}

void CPlayerLookCamera::Render(void)
{
}

CPlayerLookCamera* CPlayerLookCamera::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo)
{
	CPlayerLookCamera*		pCamera = new CPlayerLookCamera(pDevice);
	if (FAILED(pCamera->Initialize()))
		Safe_Delete(pCamera);

	pCamera->SetCameraTarget(pTargetInfo);

	return pCamera;
}

void CPlayerLookCamera::SetCameraTarget(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

void CPlayerLookCamera::Release(void)
{

}


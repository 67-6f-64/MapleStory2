#include "stdafx.h"
#include "StaticCamera.h"
#include "Transform.h"
#include "Export_Function.h"
#include "value.h"
#include "KeyMgr.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pDevice)
: CCamera(pDevice)
, m_pTargetInfo(NULL)
, m_fTargetDistance(0.f)
, m_fAngle(0.f)
, m_fCamSpeed(0.f)
{

}

CStaticCamera::~CStaticCamera(void)
{
	Release();
}

HRESULT CStaticCamera::Initialize(void)
{
	m_fTargetDistance = 20.f;
	m_fAngle = D3DXToRadian(30.f);
	m_fCamSpeed = 10.f;

	SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	return S_OK;
}

int CStaticCamera::Update(void)
{
	KeyCheck();
	TargetRenewal();

	return 0;
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo)
{
	CStaticCamera*		pCamera = new CStaticCamera(pDevice);
	if(FAILED(pCamera->Initialize()))
		Safe_Delete(pCamera);

	pCamera->SetCameraTarget(pTargetInfo);
	return pCamera;
}

void CStaticCamera::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	if(CKeyMgr::GetInstance()->StayKeyDown(DIK_UP))
		m_fAngle += D3DXToRadian(45.f) * fTime;

	if(CKeyMgr::GetInstance()->StayKeyDown(DIK_DOWN))
		m_fAngle -= D3DXToRadian(45.f) * fTime;

	if(CKeyMgr::GetInstance()->StayKeyDown(DIK_O))
		m_fTargetDistance += 10.f * fTime;

	if(CKeyMgr::GetInstance()->StayKeyDown(DIK_P))
		m_fTargetDistance -= 10.f * fTime;
}

void CStaticCamera::TargetRenewal(void)
{
	m_vEye = m_pTargetInfo->m_vDir * -1.f * m_fTargetDistance;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));
	vRight[0] = fabs(vRight[0]); // 음수 값 방지

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos;
	m_vEye += m_pTargetInfo->m_vPos;

	SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);
}

void CStaticCamera::SetCameraTarget(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
	TargetRenewal();
}

void CStaticCamera::Release(void)
{
	
}


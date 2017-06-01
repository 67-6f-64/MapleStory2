#include "stdafx.h"
#include "DynamicCamera.h"
#include "Export_Function.h"
#include "value.h"
#include "KeyMgr.h"
#include "Transform.h"
#include "Include.h"
#include "KeyMgr.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice)
	, m_vDir(g_Look)
	, m_bShowCutScene(true)
	, m_fTime(0.f)
	, m_bHandleClose(false)
{
}

CDynamicCamera::~CDynamicCamera(void)
{
	Release();
}

HRESULT CDynamicCamera::Initialize(void)
{
	SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	m_hFile = CreateFile(L"../bin/CamData/Miro.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	return S_OK;
}

int CDynamicCamera::Update(void)
{
	if (!m_bShowCutScene)
	{
		m_vEye = m_pTargetInfo->m_vPos;

		m_vEye.y += 3.f;

		m_vDir = m_pTargetInfo->m_vDir;

	}
	
	// 맨 처음 컷 씬 보여주기

	m_fTime += Engine::Get_TimeMgr()->GetTime();
	
	CutScene();
	
	m_vAt = m_vEye + m_vDir; // 카메라의 방향을 바라본다.

	SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);

	return 0;
}

void CDynamicCamera::Render(void)
{
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo)
{
	CDynamicCamera*		pCamera = new CDynamicCamera(pDevice);
	if (FAILED(pCamera->Initialize()))
		Safe_Delete(pCamera);

	pCamera->SetCameraTarget(pTargetInfo);

	return pCamera;
}

void CDynamicCamera::SetCameraTarget(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

void CDynamicCamera::CutScene(void)
{
	if (!m_bShowCutScene)
		return;

	DWORD dwByte = 0;

	CAM_INFO* pCamInfo = new CAM_INFO;

	ReadFile(m_hFile, pCamInfo, sizeof(CAM_INFO), &dwByte, NULL);

	if (dwByte == 0) // 씬 종료
	{
		Engine::Safe_Delete(pCamInfo);
		m_bShowCutScene = false;

		CloseHandle(m_hFile);
		m_bHandleClose = true;

		return;
	}

	m_vEye = pCamInfo->vecEye;
	m_vDir = pCamInfo->vecDir;

	Engine::Safe_Delete(pCamInfo);
}

void CDynamicCamera::Release(void)
{
	if (m_bHandleClose == false)
		CloseHandle(m_hFile);
}


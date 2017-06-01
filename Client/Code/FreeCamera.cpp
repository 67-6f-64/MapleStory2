#include "stdafx.h"
#include "FreeCamera.h"
#include "Export_Function.h"
#include "value.h"
#include "KeyMgr.h"
#include "Include.h"

CFreeCamera::CFreeCamera(LPDIRECT3DDEVICE9 pDevice)
	: CCamera(pDevice)
	, m_fCamSpeed(0.f)
	, m_vDir(D3DXVECTOR3(0.f, 0.f, 1.f))
	, m_bRecording(false)
	, m_bShowCutScene(true)
	, m_fRecordTime(0.f)
	, m_fTime(0.f)
{
	ZeroMemory(m_fAngle, sizeof(float) * 3);
}
CFreeCamera::~CFreeCamera(void)
{
	Release();
}

HRESULT CFreeCamera::Initialize(void)
{
	m_fCamSpeed = 45.f;

	SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	// ��Ʈ Rect
	RECT rc = { WINCX / 12, WINCY / 12, rc.left + 800, rc.top + 800 };
	m_FontRect = rc;

	RECT rc2 = { WINCX / 2, WINCY / 12, rc2.left + 800, rc2.top + 800 };
	m_TimeRect = rc2;

	// ��Ʈ
	D3DXFONT_DESC		hFont;
	ZeroMemory(&hFont, sizeof(D3DXFONT_DESC));
	hFont.Width = 50;
	hFont.Height = 50;
	hFont.Weight = FW_NORMAL;
	lstrcpy(hFont.FaceName, L"Calibri");
	hFont.CharSet = HANGEUL_CHARSET;

	HRESULT hr = D3DXCreateFontIndirect(m_pDevice, &hFont, &m_pD3DXFont);
	FAILED_CHECK(hr);

	lstrcpy(m_szRecordState, L"��ȭ�غ�");
	lstrcpy(m_szRecordTime, L"0");

	m_hFile = CreateFile(L"../bin/CamData/Stage.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	m_vEye.y = 20.f;

	return S_OK;
}

int CFreeCamera::Update(void)
{
	if (!m_bShowCutScene) // �� ���� ������ ���� ���´�
	{
		// �̵�
		KeyCheck();

		// ȸ��
		MouseCheck();
	}
	
	m_vAt = m_vEye + m_vDir; // ī�޶��� ������ �ٶ󺻴�.

	SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);

	// ��ȭ
	RecordKey();
	Record();

	// Cut-Scene
	CutScene();

	return 0;
}

void CFreeCamera::Render(void)
{
	// ��Ʈ��� 
	//m_pD3DXFont->DrawTextW(nullptr, m_szRecordState, lstrlen(m_szRecordState), &m_FontRect, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//m_pD3DXFont->DrawTextW(nullptr, m_szRecordTime, lstrlen(m_szRecordTime), &m_TimeRect, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CFreeCamera* CFreeCamera::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CFreeCamera*		pCamera = new CFreeCamera(pDevice);
	if (FAILED(pCamera->Initialize()))
		Safe_Delete(pCamera);

	return pCamera;
}

void CFreeCamera::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	D3DXVECTOR3 vecDir = { 0.f, 0.f, 0.f };

	// ��
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
	{
		vecDir = m_vDir;
		m_vEye += fTime * m_fCamSpeed * vecDir;
	}
		
	// ��
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
	{
		vecDir = -m_vDir;
		m_vEye += fTime * m_fCamSpeed * vecDir;
	}
		
	// ����
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
	{
		D3DXVec3Cross(&vecDir, &m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
		m_vEye += fTime * m_fCamSpeed * vecDir;
	}
		
	// ������
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
	{
		D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_vDir);
		m_vEye += fTime * m_fCamSpeed * vecDir;
	}

	// ī�޶� �ӵ�
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_UP))
	{
		m_fCamSpeed += fTime * 10.f;
	}
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_DOWN))
	{
		m_fCamSpeed -= fTime * 10.f;
	}
}

void CFreeCamera::MouseCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	D3DXVECTOR3 vecMouseGap = Engine::Get_Mouse()->GetMouseRelativeGap();

	// ���콺 x��ġ�� ���̷� ī�޶��� y���� ȸ����Ų�� 
	m_fAngle[Engine::ANGLE_Y] += vecMouseGap.x * fTime * 0.3f;

	// ���콺 y��ġ�� ���̷� ī�޶��� x���� ȸ����Ų��.
	m_fAngle[Engine::ANGLE_X] += vecMouseGap.y * fTime * 0.3f;

	D3DXMATRIX matRotX, matRotY;

	m_vDir = g_Look;

	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_fAngle[Engine::ANGLE_X]));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fAngle[Engine::ANGLE_Y]));

	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRotX);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRotY);
}

void CFreeCamera::RecordKey(void)
{
	// ��ȭ ����
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_F2) && FALSE == m_bRecording)
	{
		m_bRecording = true;
		lstrcpy(m_szRecordState, L"��ȭ��");

		if (m_fRecordTime == 0.f) // ��ȭ ó�� ����
		{
			time_t timer = time(NULL);
			tm t;
			localtime_s(&t, &timer);

			TCHAR szFileName[64];
			wsprintf(szFileName, L"../bin/CamData/%d��%d��%d��%d��%d��.dat", t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

			m_hFile = CreateFile(szFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		}
	}
	
	// ��ȭ �Ͻ� ����
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_F3) && TRUE == m_bRecording)
	{
		m_bRecording = false;

		lstrcpy(m_szRecordState, L"�Ͻ�����");
	}


	// ��ȭ ����
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_F4) && TRUE == m_bRecording)
	{
		m_bRecording = false;
		m_fRecordTime = 0.f;

		lstrcpy(m_szRecordState, L"��ȭ ����");

		CloseHandle(m_hFile);
	}
	
	// ��-�� �����ֱ�
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_F5) && FALSE == m_bShowCutScene)
	{
		lstrcpy(m_szRecordState, L"Cut_Scene");

		m_hFile = CreateFile(L"../bin/CamData/Stage3.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		m_bShowCutScene = true;
	}
}

void CFreeCamera::Record(void) // ��ȭ�ϱ�(F2)
{
	if (!m_bRecording)
		return;

	m_fRecordTime += Engine::Get_TimeMgr()->GetTime();

	wsprintf(m_szRecordTime, L"%d", int(m_fRecordTime));
	
	DWORD dwByte;
	CAM_INFO CamInfo = { m_vEye, m_vDir };

	m_fTime += Engine::Get_TimeMgr()->GetTime();

	WriteFile(m_hFile, &CamInfo, sizeof(CAM_INFO), &dwByte, NULL);
	
}

void CFreeCamera::CutScene(void) // �� �� �����ֱ�(F5)
{
	if (!m_bShowCutScene)
		return;

	DWORD dwByte = 0;

	CAM_INFO* pCamInfo = new CAM_INFO;

	m_fTime += Engine::Get_TimeMgr()->GetTime();

	if (m_fTime >= 0.f) // 150 ���������� �Կ�, �ƾ�
	{
		m_fTime = 0.f;

		ReadFile(m_hFile, pCamInfo, sizeof(CAM_INFO), &dwByte, NULL);

		if (dwByte == 0) // �� ����
		{
			Engine::Safe_Delete(pCamInfo);
			m_bShowCutScene = false;
			CloseHandle(m_hFile);

			lstrcpy(m_szRecordState, L"Scene_End");

			return;
		}

		m_vEye = pCamInfo->vecEye;
		m_vDir = pCamInfo->vecDir;
	}

	Engine::Safe_Delete(pCamInfo);
}

void CFreeCamera::Release(void)
{

}


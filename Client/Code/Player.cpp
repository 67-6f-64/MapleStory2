#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "KeyMgr.h"
#include "Frame.h"
#include "font.h"
#include "CubeObserver.h"

#include "ClientNetwork.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_ePlayerDir(DIR_T)
	, m_ePlayerState(PLAYER_STAND)
	, m_iTexture(0)
	, m_bJump(FALSE)
	, m_fJumpSpeed(5.f)
	, m_bThisClient(false)
{
	ZeroMemory(m_pTexture, sizeof(m_pTexture));
	lstrcpy(m_szName, L"Test");

	m_pNet = NETMGR;
}

CPlayer::~CPlayer(void)
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_bDeleteFlag = false;
	m_pInfo->m_vPos = D3DXVECTOR3(12.f, 0.f, 20.f);

	// 해당 클라이언트만 충돌체크 하면 된다
	// 벽 충돌, 빛 거리
	Engine::Get_InfoSubject()->AddData(MESSAGE_PLAYER_POSITION, &m_pInfo->m_vPos);

	return S_OK;
}

int CPlayer::Update(void)
{
	if (m_bDeleteFlag)
		return 1;
	
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);

	if (m_bThisClient)
	{
		KeyCheck();

		Jump();

		m_iTexture = (int)m_ePlayerState * 8 + (int)m_ePlayerDir;

		Engine::CGameObject::Update();

		Engine::Get_InfoSubject()->Notify(MESSAGE_PLAYER_POSITION); // 포탈과 충돌

		// 본인 클라이언트일 경우
		// 서버로 자신의 정보 Notipy
		Engine::NETDATA tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_PLAYER_NOTIPY;

		m_LocalData.matWold = m_pInfo->m_matWorld;
		m_LocalData.iTexture = m_iTexture;
		m_LocalData.dwFrame = (DWORD)m_pFrame->m_tFrame.fFrame;

		tData.matWold = m_LocalData.matWold;
		tData.dwFrame = m_LocalData.dwFrame;
		tData.iTexture = m_LocalData.iTexture;

		m_pNet->SendMsg(tData);
		
	}
	else
	{
		m_pInfo->m_matWorld = m_LocalData.matWold;

		if (m_LocalData.iTexture > 0)
			m_iTexture = m_LocalData.iTexture;

		m_pFrame->m_tFrame.fFrame = (float)m_LocalData.dwFrame;

		m_pInfo->m_vPos.x = m_LocalData.matWold._41;
		m_pInfo->m_vPos.y = m_LocalData.matWold._42;
		m_pInfo->m_vPos.z = m_LocalData.matWold._43;
	}


	return 0;
}

void CPlayer::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);
	
	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTexture[m_iTexture]->Render(0, (DWORD)m_pFrame->m_tFrame.fFrame);
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer*		pLogo = new CPlayer(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// Transform
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	ZeroMemory(&m_LocalData, sizeof(Engine::NETDATA));
	// TSW ADD
	m_LocalData.matWold = m_pInfo->m_matWorld;

	// 버퍼
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	// Frame
	pCompontent = m_pFrame = Engine::CFrame::Create(Engine::FRAME(0.f, 10.f, 10.f));
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	// Texture

	// Stand
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_B");
	m_pTexture[0] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Stand_Dir_B", pCompontent));
	
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_R");
	m_pTexture[1] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[5] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Stand_Dir_R", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_RB");
	m_pTexture[2] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[6] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Stand_Dir_RB", pCompontent));
	 
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_RT");
	m_pTexture[3] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[7] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Stand_Dir_RT", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_T");
	m_pTexture[4] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Stand_Dir_T", pCompontent));

	// Walk
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_B");
	m_pTexture[8] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Walk_Dir_B", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_R");
	m_pTexture[9] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[13] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Walk_Dir_R", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_RB");
	m_pTexture[10] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[14] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Walk_Dir_RB", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_RT");
	m_pTexture[11] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[15] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Walk_Dir_RT", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_T");
	m_pTexture[12] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Walk_Dir_T", pCompontent));

	// Jump
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_B");
	m_pTexture[16] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Jump_Dir_B", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_R");
	m_pTexture[17] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[21] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Jump_Dir_R", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_RB");
	m_pTexture[18] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[22] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Jump_Dir_RB", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_RT");
	m_pTexture[19] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[23] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Jump_Dir_RT", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_T");
	m_pTexture[20] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Player_Jump_Dir_T", pCompontent));

	return S_OK;
}

void CPlayer::Release(void)
{
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_PLAYER_POSITION, m_pInfo->m_vPos);
	//Safe_Delete(m_pFont);
}

void CPlayer::KeyCheck(void)
{
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	m_ePlayerState = PLAYER_STAND;

	D3DXMATRIX matRotY;
	D3DXVECTOR3 vecDir = m_pInfo->m_vDir;

	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
	{
		if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
		{
			m_pInfo->m_vPos += vecDir * 10.f * fTime / sqrtf(2.f);
			D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
			m_pInfo->m_vPos += vecDir * 10.f * fTime / sqrtf(2.f);;

			m_pInfo->m_vScale.x = -1.f;

			m_ePlayerState = PLAYER_WALK;
			m_ePlayerDir = DIR_LT;
		}
		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
		{
			m_pInfo->m_vPos += vecDir * 10.f * fTime / sqrtf(2.f);
			D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
			m_pInfo->m_vPos += vecDir * 10.f * fTime / sqrtf(2.f);;

			m_pInfo->m_vScale.x = 1.f;

			m_ePlayerState = PLAYER_WALK;
			m_ePlayerDir = m_ePlayerDir = DIR_RT;
		}
		else
		{
			m_pInfo->m_vPos += vecDir * 10.f * fTime;
			m_ePlayerState = PLAYER_WALK;
			m_ePlayerDir = DIR_T;
		}
	}

	else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
	{
		if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
		{
			m_pInfo->m_vPos += -vecDir * 10.f * fTime / sqrtf(2.f);
			D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
			m_pInfo->m_vPos += vecDir * 10.f * fTime / sqrtf(2.f);

			m_pInfo->m_vScale.x = -1.f;

			m_ePlayerState = PLAYER_WALK;
			m_ePlayerDir = DIR_LB;
		}
		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
		{
			m_pInfo->m_vPos += -vecDir * 10.f * fTime / sqrtf(2.f);
			D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
			m_pInfo->m_vPos += vecDir * 10.f * fTime / sqrtf(2.f);;

			m_pInfo->m_vScale.x = 1.f;

			m_ePlayerState = PLAYER_WALK;
			m_ePlayerDir = m_ePlayerDir = DIR_RB;
		}
		else
		{
			m_pInfo->m_vPos += -vecDir * 10.f * fTime;
			m_ePlayerState = PLAYER_WALK;
			m_ePlayerDir = DIR_B;
		}
	}

	else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
	{
		D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

		m_pInfo->m_vPos += -vecDir * 10.f * fTime;
		m_ePlayerState = PLAYER_WALK;
		m_ePlayerDir = DIR_L;
		m_pInfo->m_vScale.x = -1.f;
	}

	else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
	{
		D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));

		m_pInfo->m_vPos += -vecDir * 10.f * fTime;
		m_ePlayerState = PLAYER_WALK;
		m_ePlayerDir = DIR_R;
		m_pInfo->m_vScale.x = 1.f;
	}

	if (!m_bJump)
	{
		if (CKeyMgr::GetInstance()->StayKeyDown(DIK_SPACE))
		{
			m_ePlayerState = PLAYER_JUMP;
			m_bJump = true;
			m_fJumpSpeed = 5.f;
			m_pFrame->m_tFrame.fFrame = 3.f;
		}
	}
	else
		m_ePlayerState = PLAYER_JUMP;


	// TSW 미로 맵 관찰을 위해 잠시 회전 추가
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_RIGHT))
	{
		m_pInfo->m_fAnlge[Engine::ANGLE_Y] -= D3DXToRadian(90.f) * fTime;
	}

	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_LEFT))
	{
		m_pInfo->m_fAnlge[Engine::ANGLE_Y] += D3DXToRadian(90.f) * fTime;
	}
}

void CPlayer::Jump(void)
{
	if (!m_bJump)
		return;

	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_vPos.y += m_fJumpSpeed * fTime;

	m_fJumpSpeed -= 0.5f * fTime * fTime * 5000.f;

	if (m_pInfo->m_vPos.y < 0.f)
	{
		m_bJump = false;
		m_pInfo->m_vPos.y = 0.f;
	}
		
}


#include "stdafx.h"
#include "PartyFallPlayer.h"

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
#include "PartyFallCube.h"
#include "ItemObserver.h"
#include "PartyFallItem.h"

#include "ClientNetwork.h"

int CPartyFallPlayer::m_iPlayerFall = -1;

CPartyFallPlayer::CPartyFallPlayer(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pBuffer(nullptr)
, m_ePartyFallPlayerDir(DIR_T)
, m_ePartyFallPlayerState(PartyFallPlayer_STAND)
, m_iTexture(0)
, m_bJump(FALSE)
, m_fJumpSpeed(5.f)
, m_bThisClient(false)
, m_iCollideIndex(-1)
, m_bFalled(false)
, m_fFallTime(0.f)
, m_fFallSpeed(0.f)
, m_bFall(false)
, m_itemIndex(-1)
, m_fSpeed(8.f)
, m_fInitialFallTime(3.f)
{
	ZeroMemory(m_pTexture, sizeof(m_pTexture));
	m_pNet = NETMGR;
}

CPartyFallPlayer::~CPartyFallPlayer(void)
{
	Release();
}

HRESULT CPartyFallPlayer::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_bDeleteFlag = false;
	//m_pFont = CFont::Create(m_pDevice, m_szName, -1.f);
		
	// 충돌 옵저버
	Engine::Get_InfoSubject()->AddData(MESSAGE_PLAYER_POSITION, &m_pInfo->m_vPos);

	// 아이템 옵저버
	m_pPlayerData = new PLAYER_DATA;

	m_pPlayerData->m_fSpeed = &m_fSpeed;
	m_pPlayerData->vecPos = &m_pInfo->m_vPos;

	if (m_bThisClient)
		Engine::Get_InfoSubject()->AddDataFront(MESSAGE_PLAYER_DATA, m_pPlayerData);
	else
		Engine::Get_InfoSubject()->AddData(MESSAGE_PLAYER_DATA, m_pPlayerData);

	// 라운드 옵저버
	m_iTest = 0;
	m_iPlayerFall = -1;
	Engine::Get_InfoSubject()->AddData(MESSAGE_PARTYFALLPLAYER_CREATE, &m_iTest);
	Engine::Get_InfoSubject()->AddData(MESSAGE_PLAYER_FALL, &m_iPlayerFall);

	return S_OK;
}

int CPartyFallPlayer::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);

	m_fInitialFallTime -= Engine::Get_TimeMgr()->GetTime();

	if (m_pInfo->m_vPos.y == 0.f)
		Engine::Get_InfoSubject()->Notify(MESSAGE_PLAYER_POSITION); // 큐브랑 충돌
	
	if (m_bThisClient)
	{
		if(m_fInitialFallTime < 0.f)
			KeyCheck();

		// 낙하가 끝나지 않았을 때만
		if (!m_bFalled)
		{
			// 걷고 있을 때만 충돌 체크
			if (PartyFallPlayer_WALK == m_ePartyFallPlayerState)
			{
				Engine::Get_InfoSubject()->Notify(MESSAGE_PLAYER_DATA); // 아이템 충돌
			}

			// 떨어뜨리기
			Fall();

			m_iTexture = (int)m_ePartyFallPlayerState * 8 + (int)m_ePartyFallPlayerDir;

			Engine::CGameObject::Update();

			// 본인 클라이언트일 경우
			// 서버로 자신의 정보 Notipy
		}

		UpdateToServer();
	}
	else
	{
		// 낙하가 끝나지 않았을 때만
		if (!m_bFalled)
		{
			UpdateToOtherPlayer();
			Fall();
		}
	}

	return 0;
}

void CPartyFallPlayer::Render(void)
{
	// 낙하가 끝나지 않았을 때만
	if (!m_bFalled)
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
}

CPartyFallPlayer* CPartyFallPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyFallPlayer*		pLogo = new CPartyFallPlayer(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CPartyFallPlayer::AddComponent(void)
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
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Stand_Dir_B", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_R");
	m_pTexture[1] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[5] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Stand_Dir_R", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_RB");
	m_pTexture[2] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[6] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Stand_Dir_RB", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_RT");
	m_pTexture[3] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[7] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Stand_Dir_RT", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Stand_Dir_T");
	m_pTexture[4] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Stand_Dir_T", pCompontent));

	// Walk
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_B");
	m_pTexture[8] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Walk_Dir_B", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_R");
	m_pTexture[9] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[13] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Walk_Dir_R", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_RB");
	m_pTexture[10] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[14] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Walk_Dir_RB", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_RT");
	m_pTexture[11] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[15] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Walk_Dir_RT", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Walk_Dir_T");
	m_pTexture[12] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Walk_Dir_T", pCompontent));

	// Jump
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_B");
	m_pTexture[16] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Jump_Dir_B", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_R");
	m_pTexture[17] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[21] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Jump_Dir_R", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_RB");
	m_pTexture[18] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[22] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Jump_Dir_RB", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_RT");
	m_pTexture[19] = dynamic_cast<Engine::CTexture*>(pCompontent);
	m_pTexture[23] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Jump_Dir_RT", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Player_Jump_Dir_T");
	m_pTexture[20] = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"PartyFallPlayer_Jump_Dir_T", pCompontent));

	return S_OK;
}

void CPartyFallPlayer::Release(void)
{
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_PLAYER_POSITION, &m_pInfo->m_vPos);
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_PLAYER_DATA, m_pPlayerData);

	Engine::Get_InfoSubject()->RemoveData(MESSAGE_PARTYFALLPLAYER_CREATE, &m_iTest);
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_PLAYER_FALL, &m_iPlayerFall);

	Engine::Safe_Delete(m_pPlayerData);
}

void CPartyFallPlayer::KeyCheck(void)
{
	float	fTime = Engine::Get_TimeMgr()->GetTime();

	if (!m_bFalled)
	{
		m_ePartyFallPlayerState = PartyFallPlayer_WALK;
		m_ePartyFallPlayerDir = DIR_T;

		D3DXMATRIX matRotY;
		D3DXVECTOR3 vecDir = m_pInfo->m_vDir;

		// 게임이 시작되면 앞으로 계속 전진하고 A, D로 방향 전환만 가능하다.

		if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
		{
			m_ePartyFallPlayerDir = DIR_T;
		}

		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
		{
			vecDir = -m_pInfo->m_vDir;
			
			m_ePartyFallPlayerDir = DIR_B;
		}

		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
		{
			D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
			
			m_ePartyFallPlayerDir = DIR_L;
			m_pInfo->m_vScale.x = -1.f;
		}

		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
		{
			D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
			
			m_ePartyFallPlayerDir = DIR_R;
			m_pInfo->m_vScale.x = 1.f;
		}

		if (!m_bFall) // 낙하할때는 이동X
			m_pInfo->m_vPos += vecDir * m_fSpeed * fTime;
	}
	else
	{
		D3DXMATRIX matRotY;
		D3DXVECTOR3 vecDir = m_pInfo->m_vDir;

		// 낙사한 상태면 카메라이동
		if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
		{
			m_pInfo->m_vPos += vecDir * m_fSpeed * fTime;
		}

		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
		{
			vecDir = -m_pInfo->m_vDir;
			m_pInfo->m_vPos += vecDir * m_fSpeed * fTime;
		}

		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
		{
			D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
			m_pInfo->m_vPos += vecDir * m_fSpeed * fTime;
		}

		else if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
		{
			D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
			m_pInfo->m_vPos += vecDir * m_fSpeed * fTime;
		}
	}
}

void CPartyFallPlayer::UpdateToServer(void)
{
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

void CPartyFallPlayer::UpdateToOtherPlayer(void)
{
	m_pInfo->m_matWorld = m_LocalData.matWold;

	if (m_LocalData.iTexture > 0)
		m_iTexture = m_LocalData.iTexture;

	m_pFrame->m_tFrame.fFrame = (float)m_LocalData.dwFrame;

	m_pInfo->m_vPos.x = m_LocalData.matWold._41;
	m_pInfo->m_vPos.y = m_LocalData.matWold._42;
	m_pInfo->m_vPos.z = m_LocalData.matWold._43;
}

// 낙하 체크용 함수
void CPartyFallPlayer::Fall(void)
{
	// 떨어짐!
	if (m_pInfo->m_vPos.y < 0)
	{
		m_ePartyFallPlayerState = PartyFallPlayer_JUMP;
		m_bFall = true;

		m_iPlayerFall = GetServerIndex();
		Engine::Get_InfoSubject()->Notify(MESSAGE_PLAYER_FALL);
	}

	// 점프 상태일 때만 낙하시킨다
	if (PartyFallPlayer_JUMP != m_ePartyFallPlayerState)
		return;

	// 프레임이 0부터 시작하면 어색해서 0이 아니라 3부터 시작
	if (0 == m_pFrame->m_tFrame.fFrame)
		m_pFrame->m_tFrame.fFrame = 3.f;

	float fTime = Engine::Get_TimeMgr()->GetTime();
	m_fFallTime += fTime;

	// 3초동안 떨어지게 한다.
	if (3.f < m_fFallTime)
	{
		// 다 떨어지면 
		m_ePartyFallPlayerState = PartyFallPlayer_WALK;
		m_bFalled = true; // 렌더 X
		m_pInfo->m_vPos.y = 0.1f;
	
		/*m_iPlayerFall = g_iClientIndex;
		Engine::Get_InfoSubject()->Notify(MESSAGE_PLAYERFALL);*/
	}
	else
	{
		// 낙하~
		m_pInfo->m_vPos.y += m_fFallSpeed * fTime;
		m_fFallSpeed -= 0.5f * fTime * fTime * 5000.f;
	}
}

void CPartyFallPlayer::SetPlayerStartPos(void)
{
	switch (g_iClientIndex)
	{
	case 0:
		m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
		break;
	case 1:
		m_pInfo->m_vPos = D3DXVECTOR3(((PARTYFALL_CUBECNT - 1)*2.f), 0.f, 0.f);
		break;
	case 2:
		m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, ((PARTYFALL_CUBECNT - 1)*2.f));
		break;
	case 3:
		m_pInfo->m_vPos = D3DXVECTOR3(((PARTYFALL_CUBECNT - 1)*2.f), 0.f, ((PARTYFALL_CUBECNT - 1)*2.f));
		break;
	}
}
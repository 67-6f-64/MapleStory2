#include "stdafx.h"
#include "MazePlayer.h"

#include "Include.h"
#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "KeyMgr.h"
#include "font.h"

#include "ClientNetwork.h"

CMazePlayer::CMazePlayer(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_ePlayerDir(DIR_T)
	, m_ePlayerState(PLAYER_STAND)
	, m_bThisClient(false)
	, m_fSpeed(10.f)
	, m_fWalkSoundTime(0.5f)
{
	m_pNet = NETMGR;
}

CMazePlayer::~CMazePlayer()
{
	Release();
}

HRESULT CMazePlayer::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_bDeleteFlag = false;

	m_pInfo->m_vPos = D3DXVECTOR3(2.f, 1.5f, 0.f);

	m_pInfo->m_fAnlge[Engine::ANGLE_Y] = 92.5f;

	//// 충돌 옵저버
	//Engine::Get_InfoSubject()->AddDataFront(MESSAGE_PLAYER_POSITION, &m_pInfo->m_vPos);

	return S_OK;
}

int CMazePlayer::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);

	if (m_bThisClient)
	{
		KeyCheck();
		MouseCheck();

		Engine::Get_InfoSubject()->Notify(MESSAGE_PLAYER_POSITION); // 현재 위치 Notify

		Engine::CGameObject::Update();
		
		// 본인 클라이언트일 경우
		// 서버로 자신의 정보 Notipy
		Engine::NETDATA tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_PLAYER_NOTIPY;

		m_LocalData.matWold = m_pInfo->m_matWorld;

		tData.matWold = m_LocalData.matWold;
		tData.dwFrame = m_LocalData.dwFrame;
		tData.iTexture = m_LocalData.iTexture;

		m_pNet->SendMsg(tData);
	}
	else
	{
		m_pInfo->m_matWorld = m_LocalData.matWold;
		
		m_pInfo->m_vPos.x = m_LocalData.matWold._41;
		m_pInfo->m_vPos.y = m_LocalData.matWold._42;
		m_pInfo->m_vPos.z = m_LocalData.matWold._43;

		m_pInfo->m_fAnlge[Engine::ANGLE_Y] = atan2f(m_LocalData.matWold._31, m_LocalData.matWold._11);
	}	
	
	return 0;
}

void CMazePlayer::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, m_iServerIndex);
	m_pBuffer->Render();
}

CMazePlayer * CMazePlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMazePlayer*	pMazePlayer = new CMazePlayer(pDevice);
	if (FAILED(pMazePlayer->Initialize()))
	{
		Engine::Safe_Delete(pMazePlayer);
	}

	return pMazePlayer;
}

HRESULT CMazePlayer::AddComponent(void)
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
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	// 텍스쳐
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Player_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CMazePlayer::Release(void)
{
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_PLAYER_POSITION, m_pInfo->m_vPos);
}

void CMazePlayer::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_ePlayerState = PLAYER_STAND;
	D3DXVECTOR3 vecDir = { 0.f, 0.f, 0.f };

	m_fWalkSoundTime -= fTime;

	// 앞
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
	{
		vecDir = m_pInfo->m_vDir;
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;

		if (m_fWalkSoundTime < 0.f)
		{
			Engine::Get_SoundMgr()->Play_Sound(L"걷기.wav", Engine::CHANNEL_PLAYER_PISTOL);
			m_fWalkSoundTime = 0.5f;
			m_ePlayerState = PLAYER_WALK;
		}
	}

	// 뒤
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
	{
		vecDir = -m_pInfo->m_vDir;
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;

		if (m_fWalkSoundTime < 0.f)
		{
			Engine::Get_SoundMgr()->Play_Sound(L"걷기.wav", Engine::CHANNEL_PLAYER_PISTOL);
			m_fWalkSoundTime = 0.5f;
			m_ePlayerState = PLAYER_WALK;
		}
	}

	// 왼쪽
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
	{
		D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;

		if (m_fWalkSoundTime < 0.f)
		{
			Engine::Get_SoundMgr()->Play_Sound(L"걷기.wav", Engine::CHANNEL_PLAYER_PISTOL);
			m_fWalkSoundTime = 0.5f;
			m_ePlayerState = PLAYER_WALK;
		}
	}

	// 오른쪽
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
	{
		D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;

		if (m_fWalkSoundTime < 0.f)
		{
			Engine::Get_SoundMgr()->Play_Sound(L"걷기.wav", Engine::CHANNEL_PLAYER_PISTOL);
			m_fWalkSoundTime = 0.5f;
			m_ePlayerState = PLAYER_WALK;
		}
	}
}

void CMazePlayer::MouseCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	D3DXVECTOR3 vecMouseGap = Engine::Get_Mouse()->GetMouseGap();

	// 마우스 x위치의 차이로 카메라의 y축을 회전시킨다 
	m_pInfo->m_fAnlge[Engine::ANGLE_Y] += vecMouseGap.x * fTime;
}

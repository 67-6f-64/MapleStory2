#include "stdafx.h"
#include "BattleGroundPlayer.h"

#include "Include.h"
#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "KeyMgr.h"
#include "font.h"
#include "LifeCube.h"
#include "Playerhit.h"

#include "ClientNetwork.h"

CBattleGroundPlayer::CBattleGroundPlayer(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_bThisClient(false)
	, m_fSpeed(10.f)
	, m_fShootCooltime(0.f)
	, m_fShootDelay(0.f)
	, m_bCanShoot(true)
	, m_pPlayerHit(nullptr)
{
	m_bDeleteFlag = false;
	m_pNet = NETMGR;
}


CBattleGroundPlayer::~CBattleGroundPlayer()
{
	Release();
}

HRESULT CBattleGroundPlayer::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_bDeleteFlag = false;

	// 플레이어 정보
	//m_pInfo->m_vPos = D3DXVECTOR3(0.f, 1.5f, 0.f);
	m_PlayerData = BATTLE_PLAYER_DATA(m_pInfo->m_vPos, 3, m_iServerIndex);
	Engine::Get_InfoSubject()->AddData(MESSAGE_BATTLE_PLAYER_DATA, &m_PlayerData);

	// 플레이어가 가진 정보로 Bullet을 생성한다.
	m_BulletData = BULLET_DATA(m_pInfo->m_vPos, m_pInfo->m_vDir, BULLET_END, m_iServerIndex);
	Engine::Get_InfoSubject()->AddData(MESSAGE_BULLET_CREATE, &m_BulletData);

	// 아이템과 충돌
	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 1.5f, 0.f);
	m_PlayerData = BATTLE_PLAYER_DATA(m_pInfo->m_vPos, 3, m_iServerIndex);
	Engine::Get_InfoSubject()->AddData(MESSAGE_BATTLE_PLAYER_DATA_ITEM, &m_PlayerData);
	
	// 초기 라이프 3
	AddLifeCude(m_PlayerData.iLife);

	// 총쏘기 쿨타임
	m_fShootDelay = 0.5f;

	return S_OK;
}

int CBattleGroundPlayer::Update(void)
{
	if (m_bDeleteFlag)
		return 1;
	
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);
	m_BulletData = BULLET_DATA(m_pInfo->m_vPos + m_pInfo->m_vDir * 1.5f, m_pInfo->m_vDir, BULLET_NORMAL, m_iServerIndex);

	Engine::Get_InfoSubject()->Notify(MESSAGE_BATTLE_PLAYER_DATA_ITEM); // 아이템이랑 충돌은 서버통신을 안한다.
	Engine::Get_InfoSubject()->Notify(MESSAGE_BATTLE_PLAYER_DATA); // 현재 위치 Notify

	if (m_bThisClient)
	{
		KeyCheck();
		MouseCheck();

		NotMove(); // 이동 불가 지역

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

		// 서브 메세지 : 생명력
		tData.wSubMessage = m_PlayerData.iLife;

		m_pNet->SendMsg(tData);
	}
	else // 상대 클라
	{
		m_pInfo->m_matWorld = m_LocalData.matWold;

		m_pInfo->m_vPos.x = m_LocalData.matWold._41;
		m_pInfo->m_vPos.y = m_LocalData.matWold._42;
		m_pInfo->m_vPos.z = m_LocalData.matWold._43;

		m_PlayerData.iLife = m_LocalData.wSubMessage;

		m_pInfo->m_fAnlge[Engine::ANGLE_Y] = atan2f(m_LocalData.matWold._31,m_LocalData.matWold._11);
	}

	// 생명이 없으면 죽음, 남아있으면 라이프큐브 위치 업데이트
	if (m_LifeList.empty())
		;// SetDead();
	else
		SetLifeCubePos();

	// 옵저버로 보내는 정보 갱신
	m_PlayerData.vecPos = m_pInfo->m_vPos;
	m_PlayerData.iServerIndex = m_iServerIndex;

	// Life Cube 갯수 갱신

	if (0 < m_LifeList.size())
	{
		if (m_LifeList.size() < (size_t)m_PlayerData.iLife)
			AddLifeCude();
		else if (m_LifeList.size() > (size_t)m_PlayerData.iLife)
			LoseLifeCube();
	}

	PlayerHitPos();

	return 0;
}

void CBattleGroundPlayer::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, m_iServerIndex);
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 라이프 큐브 렌더
	RenderLifeCube();

	RenderPlayerHit();
}

void CBattleGroundPlayer::AddLifeCude(int iCnt)
{
	for (int i = 0; i < iCnt; ++i)
	{
		CLifeCube* pCube = CLifeCube::Create(m_pDevice);
		m_LifeList.push_back(pCube);
	}

	if (m_LifeList.size() > 1 && m_bThisClient)
		Engine::Get_SoundMgr()->StopSound(Engine::CHANNEL_PLAYER_LOWHP);
}

void CBattleGroundPlayer::LoseLifeCube(void)
{
	Engine::Safe_Delete(m_LifeList.back());
	m_LifeList.pop_back();

	if (m_LifeList.size() == 1 && m_bThisClient) // 죽기 일보 직전
		Engine::Get_SoundMgr()->PlayBGM(L"heart_beat_2.mp3", Engine::CHANNEL_PLAYER_LOWHP);
	else if(m_LifeList.size() == 0 && m_bThisClient) // 사망
		Engine::Get_SoundMgr()->StopSound(Engine::CHANNEL_PLAYER_LOWHP);

	if (m_pPlayerHit == nullptr) // Hit Effect
		m_pPlayerHit = CPlayerHit::Create(m_pDevice);
}

CBattleGroundPlayer * CBattleGroundPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBattleGroundPlayer*	pBattleGroundPlayer = new CBattleGroundPlayer(pDevice);
	if (FAILED(pBattleGroundPlayer->Initialize()))
	{
		Engine::Safe_Delete(pBattleGroundPlayer);
	}

	return pBattleGroundPlayer;
}

HRESULT CBattleGroundPlayer::AddComponent(void)
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

void CBattleGroundPlayer::Release(void)
{
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_BATTLE_PLAYER_DATA, &m_PlayerData);
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_BULLET_CREATE, &m_BulletData);

	while (!m_LifeList.empty())
		LoseLifeCube();
}

void CBattleGroundPlayer::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();
	m_fShootCooltime += fTime;

	D3DXVECTOR3 vecDir = { 0.f, 0.f, 0.f };
	m_ePlayerState = PLAYER_STAND;

	// 앞
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
	{
		vecDir = m_pInfo->m_vDir;
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// 뒤
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
	{
		vecDir = -m_pInfo->m_vDir;
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// 왼쪽
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
	{
		D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// 오른쪽
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
	{
		D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// 좌클릭
	if (CKeyMgr::GetInstance()->OnceLeftDown() && m_LifeList.size() > 0)
	{
		if (m_fShootDelay < m_fShootCooltime)
			m_bCanShoot = true;

		if (m_bCanShoot)
		{
			// 서버로 불렛 생성 메세지 전송
			Engine::NETDATA tData;
			ZeroMemory(&tData, sizeof(Engine::NETDATA));
			tData.chKey = 72;
			tData.eMessage = Engine::NETMSG_CLIENT_BULLET_CREATE_NOTIPY;
			tData.iIndex = m_iServerIndex;

			m_pNet->SendMsg(tData);

			////////////////////////////////////////////////
			// 딜레이 갱신
			m_bCanShoot = false;
			m_fShootCooltime = 0.f;

			// Sound
			Engine::Get_SoundMgr()->Play_Sound(L"sfx_pistol.wav", Engine::CHANNEL_PLAYER_PISTOL);
		}
		
	}
}

void CBattleGroundPlayer::MouseCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	D3DXVECTOR3 vecMouseGap = Engine::Get_Mouse()->GetMouseGap();

	// 마우스 x위치의 차이로 카메라의 y축을 회전시킨다 
	m_pInfo->m_fAnlge[Engine::ANGLE_Y] += vecMouseGap.x * fTime;
}

void CBattleGroundPlayer::NotMove(void)
{
	if (m_pInfo->m_vPos.x < 1)
		m_pInfo->m_vPos.x = 1.f;

	if (m_pInfo->m_vPos.x > 197.f)
		m_pInfo->m_vPos.x = 197.f;

	if (m_pInfo->m_vPos.z > 197.f)
		m_pInfo->m_vPos.z = 197.f;

	if (m_pInfo->m_vPos.z < 1)
		m_pInfo->m_vPos.z = 1.f;
}

void CBattleGroundPlayer::SetLifeCubePos(void)
{
	// 라이프 큐브가 나열될 방향벡터 구하기
	D3DXVECTOR3		vDir;
	D3DXVec3Cross(&vDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
	D3DXVec3Normalize(&vDir, &vDir);

	int iHalf = m_LifeList.size() / 2;

	list<CLifeCube*>::iterator iter = m_LifeList.begin();
	list<CLifeCube*>::iterator iter_end = m_LifeList.end();

	// 첫번째 큐브의 위치를 잡는다.
	// 먼저 플레이어 머리 위로 세팅
	(*iter)->GetTransform()->m_vPos = m_pInfo->m_vPos;
	(*iter)->GetTransform()->m_vPos.y += 1.5f;

	// 짝수개 일 때
	if (m_LifeList.size() % 2 == 0)
	{
		(*iter)->GetTransform()->m_vPos -= vDir*0.5f;
	}

	for (int i = 0; i < iHalf; ++i)
		(*iter)->GetTransform()->m_vPos += vDir*1.f;

	// 첫번째 큐브의 위치를 잡았으므로
	// 첫번째 위치 기준으로 나머지 위치 세팅
	D3DXVECTOR3 vFrist = (*iter)->GetTransform()->m_vPos;
	(*iter)->Update();
	++iter;

	for (; iter != iter_end; ++iter)
	{
		vFrist -= vDir*1.f;
		(*iter)->GetTransform()->m_vPos = vFrist;
		(*iter)->Update();
	}
}

void CBattleGroundPlayer::RenderLifeCube(void)
{
	list<CLifeCube*>::iterator iter = m_LifeList.begin();
	list<CLifeCube*>::iterator iter_end = m_LifeList.end();

	for (; iter != iter_end; ++iter)
		(*iter)->Render();

}

void CBattleGroundPlayer::PlayerHitPos(void)
{

	if (m_pPlayerHit != nullptr)
	{
		if (dynamic_cast<CPlayerHit*>(m_pPlayerHit)->GetHitFrame())
		{
			delete m_pPlayerHit;
			m_pPlayerHit = nullptr;
			return;
		}
		m_pPlayerHit->SetPos(m_pInfo->m_vPos);
		m_pPlayerHit->Update();
	}
}

void CBattleGroundPlayer::RenderPlayerHit(void)
{
	if (m_pPlayerHit != nullptr)
		m_pPlayerHit->Render();
}
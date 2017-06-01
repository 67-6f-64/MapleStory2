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

	// �÷��̾� ����
	//m_pInfo->m_vPos = D3DXVECTOR3(0.f, 1.5f, 0.f);
	m_PlayerData = BATTLE_PLAYER_DATA(m_pInfo->m_vPos, 3, m_iServerIndex);
	Engine::Get_InfoSubject()->AddData(MESSAGE_BATTLE_PLAYER_DATA, &m_PlayerData);

	// �÷��̾ ���� ������ Bullet�� �����Ѵ�.
	m_BulletData = BULLET_DATA(m_pInfo->m_vPos, m_pInfo->m_vDir, BULLET_END, m_iServerIndex);
	Engine::Get_InfoSubject()->AddData(MESSAGE_BULLET_CREATE, &m_BulletData);

	// �����۰� �浹
	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 1.5f, 0.f);
	m_PlayerData = BATTLE_PLAYER_DATA(m_pInfo->m_vPos, 3, m_iServerIndex);
	Engine::Get_InfoSubject()->AddData(MESSAGE_BATTLE_PLAYER_DATA_ITEM, &m_PlayerData);
	
	// �ʱ� ������ 3
	AddLifeCude(m_PlayerData.iLife);

	// �ѽ�� ��Ÿ��
	m_fShootDelay = 0.5f;

	return S_OK;
}

int CBattleGroundPlayer::Update(void)
{
	if (m_bDeleteFlag)
		return 1;
	
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_Look, &m_pInfo->m_matWorld);
	m_BulletData = BULLET_DATA(m_pInfo->m_vPos + m_pInfo->m_vDir * 1.5f, m_pInfo->m_vDir, BULLET_NORMAL, m_iServerIndex);

	Engine::Get_InfoSubject()->Notify(MESSAGE_BATTLE_PLAYER_DATA_ITEM); // �������̶� �浹�� ��������� ���Ѵ�.
	Engine::Get_InfoSubject()->Notify(MESSAGE_BATTLE_PLAYER_DATA); // ���� ��ġ Notify

	if (m_bThisClient)
	{
		KeyCheck();
		MouseCheck();

		NotMove(); // �̵� �Ұ� ����

		Engine::CGameObject::Update();

		// ���� Ŭ���̾�Ʈ�� ���
		// ������ �ڽ��� ���� Notipy
		Engine::NETDATA tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_PLAYER_NOTIPY;

		m_LocalData.matWold = m_pInfo->m_matWorld;

		tData.matWold = m_LocalData.matWold;
		tData.dwFrame = m_LocalData.dwFrame;
		tData.iTexture = m_LocalData.iTexture;

		// ���� �޼��� : �����
		tData.wSubMessage = m_PlayerData.iLife;

		m_pNet->SendMsg(tData);
	}
	else // ��� Ŭ��
	{
		m_pInfo->m_matWorld = m_LocalData.matWold;

		m_pInfo->m_vPos.x = m_LocalData.matWold._41;
		m_pInfo->m_vPos.y = m_LocalData.matWold._42;
		m_pInfo->m_vPos.z = m_LocalData.matWold._43;

		m_PlayerData.iLife = m_LocalData.wSubMessage;

		m_pInfo->m_fAnlge[Engine::ANGLE_Y] = atan2f(m_LocalData.matWold._31,m_LocalData.matWold._11);
	}

	// ������ ������ ����, ���������� ������ť�� ��ġ ������Ʈ
	if (m_LifeList.empty())
		;// SetDead();
	else
		SetLifeCubePos();

	// �������� ������ ���� ����
	m_PlayerData.vecPos = m_pInfo->m_vPos;
	m_PlayerData.iServerIndex = m_iServerIndex;

	// Life Cube ���� ����

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

	// ������ ť�� ����
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

	if (m_LifeList.size() == 1 && m_bThisClient) // �ױ� �Ϻ� ����
		Engine::Get_SoundMgr()->PlayBGM(L"heart_beat_2.mp3", Engine::CHANNEL_PLAYER_LOWHP);
	else if(m_LifeList.size() == 0 && m_bThisClient) // ���
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

	// ����
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	// �ؽ���
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

	// ��
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_W))
	{
		vecDir = m_pInfo->m_vDir;
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// ��
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_S))
	{
		vecDir = -m_pInfo->m_vDir;
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// ����
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_A))
	{
		D3DXVec3Cross(&vecDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// ������
	if (CKeyMgr::GetInstance()->StayKeyDown(DIK_D))
	{
		D3DXVec3Cross(&vecDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
		m_pInfo->m_vPos += fTime * m_fSpeed * vecDir;
		m_ePlayerState = PLAYER_WALK;
	}

	// ��Ŭ��
	if (CKeyMgr::GetInstance()->OnceLeftDown() && m_LifeList.size() > 0)
	{
		if (m_fShootDelay < m_fShootCooltime)
			m_bCanShoot = true;

		if (m_bCanShoot)
		{
			// ������ �ҷ� ���� �޼��� ����
			Engine::NETDATA tData;
			ZeroMemory(&tData, sizeof(Engine::NETDATA));
			tData.chKey = 72;
			tData.eMessage = Engine::NETMSG_CLIENT_BULLET_CREATE_NOTIPY;
			tData.iIndex = m_iServerIndex;

			m_pNet->SendMsg(tData);

			////////////////////////////////////////////////
			// ������ ����
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

	// ���콺 x��ġ�� ���̷� ī�޶��� y���� ȸ����Ų�� 
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
	// ������ ť�갡 ������ ���⺤�� ���ϱ�
	D3DXVECTOR3		vDir;
	D3DXVec3Cross(&vDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
	D3DXVec3Normalize(&vDir, &vDir);

	int iHalf = m_LifeList.size() / 2;

	list<CLifeCube*>::iterator iter = m_LifeList.begin();
	list<CLifeCube*>::iterator iter_end = m_LifeList.end();

	// ù��° ť���� ��ġ�� ��´�.
	// ���� �÷��̾� �Ӹ� ���� ����
	(*iter)->GetTransform()->m_vPos = m_pInfo->m_vPos;
	(*iter)->GetTransform()->m_vPos.y += 1.5f;

	// ¦���� �� ��
	if (m_LifeList.size() % 2 == 0)
	{
		(*iter)->GetTransform()->m_vPos -= vDir*0.5f;
	}

	for (int i = 0; i < iHalf; ++i)
		(*iter)->GetTransform()->m_vPos += vDir*1.f;

	// ù��° ť���� ��ġ�� ������Ƿ�
	// ù��° ��ġ �������� ������ ��ġ ����
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
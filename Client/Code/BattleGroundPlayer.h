#ifndef BattleGroundPlayer_h__
#define BattleGroundPlayer_h__

#include "GameObject.h"
#include "Include.h"
#include "Engine_Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CNetwork;
}

class CFont;
class CLifeCube;
class CPlayerHit;
class CBattleGroundPlayer
	: public Engine::CGameObject
{
private:
	explicit CBattleGroundPlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBattleGroundPlayer(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// TSW �߰�
public:
	void SetClient(void) { m_bThisClient = true; }
	void AddLifeCude(int iCnt = 1);
	void LoseLifeCube(void);

public:
	static CBattleGroundPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

	void KeyCheck(void); // �̵�
	void MouseCheck(void); // ����

	void NotMove(void); // �� �� ���� ��

	void SetLifeCubePos(void);
	void RenderLifeCube(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	float					m_fSpeed;

	// �ѽ��� ��Ÿ��
	float					m_fShootDelay;
	float					m_fShootCooltime;
	bool					m_bCanShoot;
	
private:
	bool					m_bThisClient;

	BULLET_DATA				m_BulletData;
	BATTLE_PLAYER_DATA		m_PlayerData;
	

private:
	// ������ ť��
	list<CLifeCube*>		m_LifeList;

	// ť�� �÷��̾�
public:
	PLAYER_DIR*		GetPlayerDir(void) { return &m_ePlayerDir; }
	PLAYER_STATE*	GetPlayerState(void) { return &m_ePlayerState; }

private:
	void PlayerHitPos(void);
	void RenderPlayerHit(void); 

private:
	PLAYER_DIR				m_ePlayerDir;
	PLAYER_STATE			m_ePlayerState;

	CPlayerHit*				m_pPlayerHit;
};


#endif // !BattleGroundPlayer_h__



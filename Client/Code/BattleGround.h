#ifndef BattleGround_h__
#define BattleGround_h__

#include "Scene.h"
#include "Include.h"

namespace Engine
{
	class CGameObject;
	class CNetwork;
}

class CMapObserver;
class CBulletObserver;
class CBattle_ItemObserver;

class CBattleGround
	: public Engine::CScene
{
	typedef std::map<WORD, Engine::NETDATA*> MAPPLAYERDATA;
public:
	enum LAYERID { LAYER_ENVIROMENT, LAYER_GAMELOGIC, LAYER_UI };
	enum eFLAGID { FLAG_PLAYER_CREATE, FLAG_PLAYER_UPDATE, FLAG_BULLET_CREATE, FLAG_END };

private:
	explicit CBattleGround(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBattleGround(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CBattleGround* Create(LPDIRECT3DDEVICE9 pDevice);

	// 아이템 랜덤 자동 생성
	void SpawnItem(void);
	// 메테오 랜덤 생성
	void SpawnMeteor(void);

private:
	HRESULT	Initialize(void);

	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	void Release(void);

	void SetPlayerStartPos(Engine::CGameObject* pObj);

	/////////////////////////////////////////////////////
	// 플래그에 맞춰 실행시킬 함수
	void FlagFuntion(void);

	// 플레이어 생성
	void PlayerCreate(void);

	// 플레이어 업데이트
	void PlayerUpdate(void);

	// 불렛 생성
	void BulletCreate(void);

private:
	Engine::CNetwork*		m_pNet;
	CBulletObserver*		m_pBulletObserver;
	CBattle_ItemObserver*	m_pItemObserver;
	CMapObserver*			m_pMapObserver;

	float				m_fSpawnTime;
	float				m_fMeteorTime;

	int					m_iBulletCreatePlayerIndex;

	HANDLE				m_fItemHandle;
	HANDLE				m_fMeteorHandle;

	
};

#endif
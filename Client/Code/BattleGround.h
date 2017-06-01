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

	// ������ ���� �ڵ� ����
	void SpawnItem(void);
	// ���׿� ���� ����
	void SpawnMeteor(void);

private:
	HRESULT	Initialize(void);

	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	void Release(void);

	void SetPlayerStartPos(Engine::CGameObject* pObj);

	/////////////////////////////////////////////////////
	// �÷��׿� ���� �����ų �Լ�
	void FlagFuntion(void);

	// �÷��̾� ����
	void PlayerCreate(void);

	// �÷��̾� ������Ʈ
	void PlayerUpdate(void);

	// �ҷ� ����
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
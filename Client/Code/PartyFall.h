#ifndef PartyFall_h__
#define PartyFall_h__

#include "Scene.h"
#include "Include.h"

namespace Engine 
{
	class CGameObject;
	class CNetwork;
}

class CCubeObserver;
class CItemObserver;
class CRoundObserver;
class CPartyFall :
	public Engine::CScene
{
	typedef std::map<WORD, Engine::NETDATA*> MAPPLAYERDATA;
public:
	enum LAYERID { LAYER_ENVIROMENT, LAYER_GAMELOGIC, LAYER_UI };
	enum eFLAGID { FLAG_PLAYER_CREATE, FLAG_PLAYER_UPDATE, FLAG_END };

private:
	explicit CPartyFall(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyFall(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CPartyFall* Create(LPDIRECT3DDEVICE9 pDevice);

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

public:
	HRESULT Round_Start_UI(void);
	HRESULT Round_Player_Win(int iPlayerWin);

private:
	Engine::CNetwork*	m_pNet;
	eGameState  m_eGameState;

	float m_fTime; // 3초
	int m_iPlayerCount;

	bool m_bRoundFinish;

	static int		m_iRound;

	CCubeObserver* m_pCubeObserver;
	CItemObserver* m_pItemObserver;
	CRoundObserver* m_pRoundObserver;
};

#endif
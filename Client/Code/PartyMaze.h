#ifndef PartyMaze_h__
#define PartyMaze_h__

#include "Scene.h"

namespace Engine 
{
	class CNetwork;
}

class CMazePosObserver;
class CPartyMaze
	: public Engine::CScene
{
	typedef std::map<WORD, Engine::NETDATA*> MAPPLAYERDATA;
public:
	enum LAYERID { LAYER_ENVIROMENT, LAYER_GAMELOGIC, LAYER_UI };
	enum eFLAGID { FLAG_PLAYER_CREATE, FLAG_PLAYER_UPDATE, FLAG_END };

private:
	explicit CPartyMaze(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyMaze(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CPartyMaze* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT	Initialize(void);

	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	void Release(void);

	void Add_Light(D3DXVECTOR3 vecPos, int iLightCount);

	// 승리
	HRESULT Round_Player_Win(int iPlayerWin);

	/////////////////////////////////////////////////////
	// 플래그에 맞춰 실행시킬 함수
	void FlagFuntion(void);

	// 플레이어 생성
	void PlayerCreate(void);

	// 플레이어 업데이트
	void PlayerUpdate(void);

private:

	Engine::CNetwork*	m_pNet;

	CMazePosObserver*	m_pMazePosObserver;
	
	float				m_fTime;
};

#endif // PartyMaze_h__
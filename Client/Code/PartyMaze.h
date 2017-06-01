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

	// �¸�
	HRESULT Round_Player_Win(int iPlayerWin);

	/////////////////////////////////////////////////////
	// �÷��׿� ���� �����ų �Լ�
	void FlagFuntion(void);

	// �÷��̾� ����
	void PlayerCreate(void);

	// �÷��̾� ������Ʈ
	void PlayerUpdate(void);

private:

	Engine::CNetwork*	m_pNet;

	CMazePosObserver*	m_pMazePosObserver;
	
	float				m_fTime;
};

#endif // PartyMaze_h__
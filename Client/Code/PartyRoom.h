#ifndef PartyRoom_h__
#define PartyRoom_h__

#include "Scene.h"

namespace Engine
{
	class CNetwork;
}

class CPortalObserver;
class CPartyRoom
	: public Engine::CScene
{
	typedef std::map<WORD, Engine::NETDATA*> MAPPLAYERDATA;
public:
	enum LAYERID { LAYER_ENVIROMENT, LAYER_GAMELOGIC, LAYER_UI };
	enum eFLAGID { FLAG_PLAYER_CREATE, FLAG_PLAYER_UPDATE, FLAG_END };

private:
	explicit CPartyRoom(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyRoom(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CPartyRoom* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT	Initialize(void);

	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	void Release(void);

	void Add_Light(D3DXVECTOR3 vecPos);

	/////////////////////////////////////////////////////
	// �÷��׿� ���� �����ų �Լ�
	void FlagFuntion(void);

	// �÷��̾� ����
	void PlayerCreate(void);

	// �÷��̾� ������Ʈ
	void PlayerUpdate(void);

private:
	int			m_iLightCount;
	Engine::CNetwork*	m_pNet;

	CPortalObserver*	m_pPortalObserver;
};

#endif // PartyRoom_h__
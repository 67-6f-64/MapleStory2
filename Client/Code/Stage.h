/*!
* \file Stage.h
* \date 2017/04/27 16:48
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

namespace Engine
{
	class CNetwork;
}

class CStage
	: public Engine::CScene
{
	typedef std::map<WORD, Engine::NETDATA*> MAPPLAYERDATA;
public:
	enum LAYERID { LAYER_ENVIROMENT, LAYER_GAMELOGIC, LAYER_UI };
	enum eFLAGID { FLAG_PLAYER_CREATE, FLAG_PLAYER_UPDATE, FLAG_END };

private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStage(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT	Initialize(void);
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);

	void Release(void);

	/////////////////////////////////////////////////////
	// 플래그에 맞춰 실행시킬 함수
	void FlagFuntion(void);

	// 플레이어 생성
	void PlayerCreate(void);

	// 플레이어 업데이트
	void PlayerUpdate(void);

private:
	int					m_iLightCount;
	Engine::CNetwork*	m_pNet;
};


#endif // Stage_h__
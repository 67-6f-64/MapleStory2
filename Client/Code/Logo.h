/*!
* \file Logo.h
* \date 2017/04/27 16:41
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

#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"

namespace Engine
{
	class CNetwork;
}

class CLogo
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_ENVIROMENT, LAYER_GAMELOGIC, LAYER_UI };

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogo(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT	Initialize(void);
	HRESULT Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_UI_Layer(void);
	void Release(void);

private:
	HRESULT Add_Buffer(void);
	HRESULT Add_Texture(void);

	HRESULT Add_Texture_Map(void);
	HRESULT Add_Texture_Player(void);
	HRESULT Add_Textrue_UI(void);
	HRESULT Add_Textrue_Effect(void);
	HRESULT Add_Textrue_ETC(void);

private:
	void LoadThread(void);

private:
	Engine::CNetwork*		m_pNet;

	bool					m_bLodeFinish;
};

#endif // Logo_h__
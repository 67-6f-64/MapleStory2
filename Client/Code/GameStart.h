/*!
* \file LogoBack.h
* \date 2017/04/28 15:52
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

#ifndef GameStart_h__
#define GameStart_h__

#include "GameObject.h"
#include "Include.h"
#include "Engine_Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CGameStart
	: public Engine::CGameObject
{
private:
	explicit CGameStart(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGameStart(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CGameStart* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	Engine::CFrame*			m_pFrame;
	int						m_iTexture;
};

#endif // LogoBack_h__
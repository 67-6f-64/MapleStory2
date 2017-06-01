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

#ifndef LogoBack_h__
#define LogoBack_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CLogoBack
	: public Engine::CGameObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogoBack(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__
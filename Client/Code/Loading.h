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

#ifndef Loading_h__
#define Loading_h__

#include "GameObject.h"
#include "Include.h"
#include "Engine_Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CLoading
	: public Engine::CGameObject
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLoading(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pDevice);

	static int GetCount() { return iCount; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	Engine::CFrame*			m_pFrame;

	static int iCount;
	bool m_bCount;
};

#endif // LogoBack_h__
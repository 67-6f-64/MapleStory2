

#ifndef MainUI_h__
#define MainUI_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CMainUI
	: public Engine::CGameObject
{
private:
	explicit CMainUI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMainUI(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CMainUI* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	D3DXMATRIX				m_matIdentity;
	D3DXMATRIX				m_matOrthoProj;
};

#endif // LogoBack_h__
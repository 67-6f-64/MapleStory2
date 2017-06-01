

#ifndef PlayerWin_h__
#define PlayerWin_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPlayerWin
	: public Engine::CGameObject
{
private:
	explicit CPlayerWin(LPDIRECT3DDEVICE9 pDevice, int iPlayerWin);

public:
	virtual ~CPlayerWin(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CPlayerWin* Create(LPDIRECT3DDEVICE9 pDevice, int iPlayerWin);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	D3DXMATRIX				m_matIdentity;
	D3DXMATRIX				m_matOrthoProj;

	int						m_iPlayer;
	float					m_fTime;
	
};

#endif // LogoBack_h__
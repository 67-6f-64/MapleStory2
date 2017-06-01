

#ifndef PartyFallNumber_h__
#define PartyFallNumber_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyFallNumber
	: public Engine::CGameObject
{
private:
	explicit CPartyFallNumber(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyFallNumber(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CPartyFallNumber* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	D3DXMATRIX				m_matIdentity;
	D3DXMATRIX				m_matOrthoProj;

	float					m_fTime;
};

#endif // LogoBack_h__
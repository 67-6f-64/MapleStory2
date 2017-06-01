

#ifndef PartyFallBackground_h__
#define PartyFallBackground_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyFallBackground
	: public Engine::CGameObject
{
private:
	explicit CPartyFallBackground(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyFallBackground(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CPartyFallBackground* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__
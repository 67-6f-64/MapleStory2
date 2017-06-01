

#ifndef PartyRoomBackgroundBackground_h__
#define PartyRoomBackgroundBackground_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyRoomBackground
	: public Engine::CGameObject
{
private:
	explicit CPartyRoomBackground(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyRoomBackground(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CPartyRoomBackground* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__
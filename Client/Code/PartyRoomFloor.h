#ifndef PartyRoomFloor_h__
#define PartyRoomFloor_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyRoomFloor
	: public Engine::CGameObject
{
private:
	explicit CPartyRoomFloor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyRoomFloor(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPartyRoomFloor* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
};

#endif // LogoBack_h__
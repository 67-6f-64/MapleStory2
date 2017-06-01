

#ifndef Tile_h__
#define Tile_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CTile
	: public Engine::CGameObject
{
private:
	explicit CTile(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTile(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CTile* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__


#ifndef Ceiling_h__
#define Ceiling_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CCeiling
	: public Engine::CGameObject
{
private:
	explicit CCeiling(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCeiling(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CCeiling* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__
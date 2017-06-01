#ifndef Floor_h__
#define Floor_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CFloor
	: public Engine::CGameObject
{
private:
	explicit CFloor(LPDIRECT3DDEVICE9 pDevice, WORD wTexture = 0);

public:
	virtual ~CFloor(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CFloor* Create(LPDIRECT3DDEVICE9 pDevice, WORD wTexture = 0);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	WORD					m_wTexture;
};

#endif // LogoBack_h__
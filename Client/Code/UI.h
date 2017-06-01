

#ifndef HpMpGauge_h__
#define HpMpGauge_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CHpMpGauge
	: public Engine::CGameObject
{
private:
	explicit CHpMpGauge(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHpMpGauge(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CHpMpGauge* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
};

#endif // LogoBack_h__

#ifndef Portal_h__
#define Portal_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CPortal
	: public Engine::CGameObject
{
private:
	explicit CPortal(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPortal(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pDevice, ePORTALID eId);
	void SetPortalId(ePORTALID eId) { m_ePortalId = eId; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pUITexture;

	Engine::CTexture*		m_pTexture;

	Engine::CFrame*			m_pFrame;
	Engine::CFrame*			m_pUIFrame;

	ePORTALID				m_ePortalId;
};

#endif // Portal_h__
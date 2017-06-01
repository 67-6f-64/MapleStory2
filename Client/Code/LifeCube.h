#ifndef LifeCube_h__
#define LifeCube_h__

#include "GameObject.h"


namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CTransform;
}

class CLifeCube : public Engine::CGameObject
{
private:
	explicit CLifeCube(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLifeCube(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CLifeCube* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
};

#endif
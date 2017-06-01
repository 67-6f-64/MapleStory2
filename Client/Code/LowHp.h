

#ifndef LowHp_h__
#define LowHp_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CLowHp
	: public Engine::CGameObject
{
private:
	explicit CLowHp(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLowHp(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CLowHp* Create(LPDIRECT3DDEVICE9 pDevice);

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
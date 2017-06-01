

#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CSkyBox
	: public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pDevice, SKYBOX_ID eId);

public:
	virtual ~CSkyBox(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pDevice, SKYBOX_ID eId);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	SKYBOX_ID				m_eSkyBoxId;
};

#endif // LogoBack_h__
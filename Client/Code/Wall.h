

#ifndef Wall_h__
#define Wall_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CWall
	: public Engine::CGameObject
{
	enum WALL_DIR{ WALL_LEFT, WALL_RIGHT, WALL_BACK, WALL_FRONT, WALL_END};

private:
	explicit CWall(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWall(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CWall* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer[WALL_END];
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__
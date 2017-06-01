#ifndef MazeWall_h__
#define MazeWall_h__

#include "GameObject.h"
#include "Enum.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CMazeWall	: public Engine::CGameObject
{
private:
	explicit CMazeWall(LPDIRECT3DDEVICE9 pDevice, WORD wDir);

public:
	virtual ~CMazeWall(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CMazeWall* Create(LPDIRECT3DDEVICE9 pDevice, WORD wDir = 0);

public:
	WORD	GetDir(void) { return m_wDirection; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer[WALL_END];
	Engine::CTexture*		m_pTexture;

	WORD					m_wDirection;
};

#endif
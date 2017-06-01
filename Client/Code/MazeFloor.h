#ifndef MazeFloor_h__
#define MazeFloor_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CMazeFloor
	: public Engine::CGameObject
{
private:
	explicit CMazeFloor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMazeFloor(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CMazeFloor* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	ID3DXFont*				m_pD3DXFont;
	RECT					m_FontRect;
	TCHAR					m_szTime[16];
	float					m_fTime;
};

#endif // LogoBack_h__
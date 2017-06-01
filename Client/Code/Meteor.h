
#ifndef Meteor_h__
#define Meteor_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CMeteorEffect;
class CMeteor
	: public Engine::CGameObject
{
private:
	explicit CMeteor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMeteor(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CMeteor* Create(LPDIRECT3DDEVICE9 pDevice);

	void Fall(void);
	bool GetSound(void) { return m_bSound; }
	void SetSound(void) { m_bSound = false; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CFrame*			m_pFrame;

	CMeteorEffect*			m_pEffect;

	bool					m_bSound;
};

#endif // Meteor_h__
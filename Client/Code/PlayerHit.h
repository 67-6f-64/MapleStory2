#pragma once

#ifndef PlayerHit_h__
#define PlayerHit_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CPlayerHit
	: public Engine::CGameObject
{
private:
	explicit CPlayerHit(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayerHit(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPlayerHit* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	bool GetHitFrame(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	Engine::CFrame*			m_pFrame;

	bool			m_bIsEnd;
};

#endif // PlayerHit_h__
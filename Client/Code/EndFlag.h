#pragma once
#pragma once

#ifndef EndFlag_h__
#define EndFlag_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CEndFlag
	: public Engine::CGameObject
{
private:
	explicit CEndFlag(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CEndFlag(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CEndFlag* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	Engine::CFrame*			m_pFrame;
};

#endif // EndFlag_h__
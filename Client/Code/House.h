#ifndef House_h__
#define House_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CHouse
	: public Engine::CGameObject
{
private:
	explicit CHouse(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHouse(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	void SetHouseColor(int iNum) { m_iColor = iNum; }

public:
	static CHouse* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	int						m_iColor;
};

#endif 
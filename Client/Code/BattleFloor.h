#ifndef BattleFloor_h__
#define BattleFloor_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CBattleFloor
	: public Engine::CGameObject
{
private:
	explicit CBattleFloor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBattleFloor(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CBattleFloor* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
};

#endif // LogoBack_h__
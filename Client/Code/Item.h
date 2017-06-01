
#ifndef Item_h__
#define Item_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CItem
	: public Engine::CGameObject
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CItem(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pDevice);

	void SetItemType(eITEMID eId) { m_eItemId = eId; }
	eITEMID GetItemType(void) { return m_eItemId; }

	void Fall(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	eITEMID					m_eItemId;
};

#endif // Item_h__
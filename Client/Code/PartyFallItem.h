

#ifndef PartyFallItem_h__
#define PartyFallItem_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyFallItem
	: public Engine::CGameObject
{
private:
	explicit CPartyFallItem(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyFallItem(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPartyFallItem* Create(LPDIRECT3DDEVICE9 pDevice);

	void Fall(); // ���ʿ� ������ ��������.

	bool IsCollide(void) { return m_bCollide; }
	void SetCollide(bool b) { m_bCollide = b; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	bool					m_bCollide; // �浹 �ߴ���
};

#endif // LogoBack_h__
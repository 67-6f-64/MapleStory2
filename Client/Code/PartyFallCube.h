

#ifndef PartyFallCube_h__
#define PartyFallCube_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyFallCube
	: public Engine::CGameObject
{
private:
	explicit CPartyFallCube(LPDIRECT3DDEVICE9 pDevice, int iTexture);

public:
	virtual ~CPartyFallCube(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// ���� ��ġ ����
	void SetInitialPos(D3DXVECTOR3 vecPos) { m_vecInitialPos = vecPos; }

	// �ٽ� ���� ��ġ��(���� ���� ��)
	void RollBackPos(void); 

public:
	static CPartyFallCube* Create(LPDIRECT3DDEVICE9 pDevice, int iTexture);
	
public:
	void Collide(void);
	void Fall(void);

	// �÷��̾�� �浹!
	void SetCollide(void) { m_bCollide = true; }
	bool IsCollide(void) {	return m_bCollide; }

	// �������� ť������!
	bool bIsCubeFall(void) { return m_bFall; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	int						m_iTexture;
	bool					m_bCollide; // �÷��̾�� �浹�� �ߴ���
	bool					m_bFall; // ť�� ����
	float					m_fTime; // ��鸮�� �ð�

	float					m_fShakeSpeed; // ��鸮�� �ӵ�
	bool					m_bShakeLeft; // �������� ��鸮����

	D3DXVECTOR3				m_vecInitialPos; // ������ ��ġ (���� ����� �� �� ���� ��ġ�� ���ư��� ���ؼ�)
};

#endif // LogoBack_h__
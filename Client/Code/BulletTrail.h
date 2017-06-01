#ifndef BulletTrail_h__
#define BulletTrail_h__

#include "GameObject.h"


namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CTransform;
}

class CBulletTrail : public Engine::CGameObject
{
private:
	explicit CBulletTrail(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBulletTrail(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CBulletTrail* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

	void LiveTimeCheck(float fTime);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	float					m_fLiveTime;		// ���� ����ִ� �ð�
	float					m_fMaxLiveTime;		// �ִ� ����ִ� �ð�
	float					m_fOriScale;		// �ð��� ���� ũ�������� ���� ������ �ʱ� ũ�Ⱚ
};

#endif
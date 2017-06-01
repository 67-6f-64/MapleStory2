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
	float					m_fLiveTime;		// 현재 살아있는 시간
	float					m_fMaxLiveTime;		// 최대 살아있는 시간
	float					m_fOriScale;		// 시간에 따라 크기조절을 위해 설정된 초기 크기값
};

#endif
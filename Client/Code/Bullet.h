
#ifndef Bullet_h__
#define Bullet_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CBulletTrail;
class CBullet
	: public Engine::CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBullet(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

	void Move(void);
	int Disappear(void);

	void TrailUpdate(void);
	void TrailRender(void);
	void TrailRelease(void);

public:
	// 총알의 위치, 방향, 종류 설정
	void SetData(BULLET_DATA BulletData);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::CFrame*			m_pFrame;

private:
	list<CBulletTrail*>		m_listTrail;
};

#endif // Bullet_h__
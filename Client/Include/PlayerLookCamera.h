

#ifndef PlayerLookCamera_h__
#define PlayerLookCamera_h__

#include "Camera.h"

namespace Engine
{
	class CTransform;
}

class CPlayerLookCamera
	: public Engine::CCamera
{
private:
	explicit CPlayerLookCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayerLookCamera(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	void SetCameraTarget(const Engine::CTransform* pTargetInfo);

public:
	static CPlayerLookCamera* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo);

private:
	void Release(void);

private:
	const Engine::CTransform*		m_pTargetInfo;

	D3DXVECTOR3		m_vDir;
};

#endif // PlayerLookCamera_h__

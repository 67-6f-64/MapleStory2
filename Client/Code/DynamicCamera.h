

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

namespace Engine
{
	class CTransform;
}

class CDynamicCamera
	: public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDynamicCamera(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	void SetCameraTarget(const Engine::CTransform* pTargetInfo);

	// ÄÆ ¾À
	void CutScene(void);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo);

private:
	void Release(void);

private:
	const Engine::CTransform*		m_pTargetInfo;

	D3DXVECTOR3		m_vDir;

	bool        m_bShowCutScene;
	HANDLE		m_hFile;

	float		m_fTime;

	bool		m_bHandleClose;
};

#endif // DynamicCamera_h__

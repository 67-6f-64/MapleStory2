#ifndef FrustumMgr_h__
#define FrustumMgr_h__

#include "Engine_Include.h"

class CFrustumMgr
{
public:
	DECLARE_SINGLETON(CFrustumMgr)

private:
	CFrustumMgr(void);
	~CFrustumMgr(void);

public:
	void Initialize(void);

	void MakeFrustumPlane(D3DXMATRIX matView, D3DXMATRIX matProj);

	bool IsPointInFrustum(D3DXVECTOR3 vecPos);
	bool IsSphereInFrustum(D3DXVECTOR3 vecPos, float fRadius);

private:
	D3DXPLANE			 m_Plane[6];
	D3DXVECTOR3			 m_vecFrustumViewPort[8];
	D3DXVECTOR3			 m_vecFrustumWorld[8];
};

#endif // FrustumMgr_h__
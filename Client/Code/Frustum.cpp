#include "stdafx.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CFrustumMgr)

CFrustumMgr::CFrustumMgr(void)
{
	Initialize();
}

CFrustumMgr::~CFrustumMgr(void)
{

}

void CFrustumMgr::Initialize(void)
{
	// (-1, -1, 0) ~ (1, 1, 1)
	// 아래
	m_vecFrustumViewPort[0].x = -1.0f; m_vecFrustumViewPort[0].y = -1.0f; m_vecFrustumViewPort[0].z = 0.0f;
	m_vecFrustumViewPort[1].x = 1.0f; m_vecFrustumViewPort[1].y = -1.0f; m_vecFrustumViewPort[1].z = 0.0f;
	m_vecFrustumViewPort[2].x = 1.0f; m_vecFrustumViewPort[2].y = -1.0f; m_vecFrustumViewPort[2].z = 1.0f;
	m_vecFrustumViewPort[3].x = -1.0f; m_vecFrustumViewPort[3].y = -1.0f; m_vecFrustumViewPort[3].z = 1.0f;

	// 위
	m_vecFrustumViewPort[4].x = -1.0f; m_vecFrustumViewPort[4].y = 1.0f; m_vecFrustumViewPort[4].z = 0.0f;
	m_vecFrustumViewPort[5].x = 1.0f; m_vecFrustumViewPort[5].y = 1.0f; m_vecFrustumViewPort[5].z = 0.0f;
	m_vecFrustumViewPort[6].x = 1.0f; m_vecFrustumViewPort[6].y = 1.0f; m_vecFrustumViewPort[6].z = 1.0f;
	m_vecFrustumViewPort[7].x = -1.0f; m_vecFrustumViewPort[7].y = 1.0f; m_vecFrustumViewPort[7].z = 1.0f;
}

void CFrustumMgr::MakeFrustumPlane(D3DXMATRIX matView, D3DXMATRIX matProj)
{
	D3DXMATRIX matViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matViewProj, nullptr, &matViewProj); 

	// matView와 matProj를 곱하면 (-1, -1, 0) ~ (1, 1, 1) 사이의 점이 된다.
	// (-1, -1, 0) ~ (1, 1, 1) 사이의 점에 그 역행렬을 곱하면 월드 좌표의 Frustum을 구할 수 있다.

	for(int i=0; i<8; ++i)
		D3DXVec3TransformCoord(&m_vecFrustumWorld[i], &m_vecFrustumViewPort[i], &matViewProj);

	D3DXPlaneFromPoints(&m_Plane[0], m_vecFrustumWorld + 4, m_vecFrustumWorld + 7, m_vecFrustumWorld + 6); // 상 평면(top)
	D3DXPlaneFromPoints(&m_Plane[1], m_vecFrustumWorld , m_vecFrustumWorld+1, m_vecFrustumWorld+2); // 하 평면(bottom)
	D3DXPlaneFromPoints(&m_Plane[2], m_vecFrustumWorld , m_vecFrustumWorld+4, m_vecFrustumWorld+5); // 근 평면(near)
	D3DXPlaneFromPoints(&m_Plane[3], m_vecFrustumWorld + 2, m_vecFrustumWorld + 6, m_vecFrustumWorld + 7); // 원 평면(far)
	D3DXPlaneFromPoints(&m_Plane[4], m_vecFrustumWorld, m_vecFrustumWorld + 3, m_vecFrustumWorld + 7); // 좌 평면(left)
	D3DXPlaneFromPoints(&m_Plane[5], m_vecFrustumWorld + 1, m_vecFrustumWorld + 5, m_vecFrustumWorld + 6); // 우 평면(right)

}

bool CFrustumMgr::IsPointInFrustum(D3DXVECTOR3 vecPos)
{
	for (int i = 0; i < 6; ++i)
	{
		float fDistance = D3DXPlaneDotCoord(&m_Plane[i], &vecPos);

		if (fDistance > 0)
			return false;
	}
	
	return true;
}

bool CFrustumMgr::IsSphereInFrustum(D3DXVECTOR3 vecPos, float fRadius)
{
	for (int i = 0; i < 6; ++i)
	{
		float fDistance = D3DXPlaneDotCoord(&m_Plane[i], &vecPos);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}




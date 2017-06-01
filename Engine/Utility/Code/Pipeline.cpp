#include "Pipeline.h"
#include "MathMgr.h"

Engine::CPipeline::CPipeline(void)
{

}

Engine::CPipeline::~CPipeline(void)
{

}

void Engine::CPipeline::MakeWorldMatrix(D3DXMATRIX* pOut
	, const D3DXVECTOR3* pScale
	, const float* pAngle
	, const D3DXVECTOR3* pPos)
{
	D3DXMatrixIdentity(pOut);

	D3DXVECTOR3		vRight = D3DXVECTOR3(1.f, 0.f, 0.f); //1행
	D3DXVECTOR3		vUp = D3DXVECTOR3(0.f, 1.f, 0.f); //2행
	D3DXVECTOR3		vLook = D3DXVECTOR3(0.f, 0.f, 1.f); //3행
	D3DXVECTOR3		vPos = D3DXVECTOR3(0.f, 0.f, 0.f); //4행

	//스케일
	vRight *= pScale->x;
	vUp *= pScale->y;
	vLook *= pScale->z;

	//자전 x -> y -> z
	//CMathMgr::MyRotationX(&vRight, &vRight, pAngle[ANGLE_X]);

	CMathMgr::MyRotationX(&vUp, &vUp, pAngle[ANGLE_X]);
	CMathMgr::MyRotationX(&vLook, &vLook, pAngle[ANGLE_X]);

	CMathMgr::MyRotationY(&vRight, &vRight, pAngle[ANGLE_Y]);
	//CMathMgr::MyRotationY(&vUp, &vUp, pAngle[ANGLE_Y]);
	CMathMgr::MyRotationY(&vLook, &vLook, pAngle[ANGLE_Y]);

	CMathMgr::MyRotationZ(&vRight, &vRight, pAngle[ANGLE_Z]);
	CMathMgr::MyRotationZ(&vUp, &vUp, pAngle[ANGLE_Z]);
	//CMathMgr::MyRotationZ(&vLook, &vLook, pAngle[ANGLE_Z]);

	//이동
	vPos = *pPos;

	MakeTransformMatrix(pOut, &vRight, &vUp, &vLook, &vPos);
}

void Engine::CPipeline::MakeTransformMatrix(D3DXMATRIX* pOut, const D3DXVECTOR3* pRight, const D3DXVECTOR3* pUp, const D3DXVECTOR3* pLook, const D3DXVECTOR3* pPos)
{
	memcpy(&pOut->m[0][0], pRight, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[1][0], pUp, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[2][0], pLook, sizeof(D3DXVECTOR3));
	memcpy(&pOut->m[3][0], pPos, sizeof(D3DXVECTOR3));
}

// 사용 안함
void Engine::CPipeline::MakeViewSpaceMatrix(D3DXMATRIX* pOut, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp)
{
	D3DXMatrixIdentity(pOut);

	D3DXVECTOR3		vRight = D3DXVECTOR3(1.f, 0.f, 0.f); //1행
	D3DXVECTOR3		vUp = D3DXVECTOR3(0.f, 1.f, 0.f); //2행
	D3DXVECTOR3		vLook = D3DXVECTOR3(0.f, 0.f, 1.f); //3행
	D3DXVECTOR3		vPos = D3DXVECTOR3(0.f, 0.f, 0.f); //4행

													   //Look
	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	//Right
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	//Up
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vPos = *pEye;

	MakeTransformMatrix(pOut, &vRight, &vUp, &vLook, &vPos);
	D3DXMatrixInverse(pOut, NULL, pOut);
}

// 사용 안함
void Engine::CPipeline::MakeProjectionMatrix(D3DXMATRIX* pOut, const float& fFovY, const float& fAspect, const float& fNear, const float& fFar)
{
	D3DXMatrixIdentity(pOut);

	pOut->_11 = (1.f / tanf(fFovY / 2.f)) / fAspect;
	pOut->_22 = 1.f / tanf(fFovY / 2.f);

	pOut->_33 = fFar / (fFar - fNear);
	pOut->_43 = -fNear * fFar / (fFar - fNear);

	pOut->_34 = 1.f;
	pOut->_44 = 0.f;
}

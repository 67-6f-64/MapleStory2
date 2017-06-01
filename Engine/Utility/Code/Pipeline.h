/*!
* \file Pipeline.h
* \date 2017/05/01 15:32
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef Pipeline_h__
#define Pipeline_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline
{
public:
	CPipeline(void);
	~CPipeline(void);

public:
	static void MakeWorldMatrix(D3DXMATRIX* pOut
		, const D3DXVECTOR3* pScale
		, const float* pAngle
		, const D3DXVECTOR3* pPos);

	static void MakeViewSpaceMatrix(D3DXMATRIX* pOut
		, const D3DXVECTOR3* pEye
		, const D3DXVECTOR3* pAt
		, const D3DXVECTOR3* pUp);

	static void MakeProjectionMatrix(D3DXMATRIX* pOut
		, const float& fFovY
		, const float& fAspect
		, const float& fNear
		, const float& fFar);

	static void MakeTransformMatrix(D3DXMATRIX* pOut
		, const D3DXVECTOR3* pRight
		, const D3DXVECTOR3* pUp
		, const D3DXVECTOR3* pLook
		, const D3DXVECTOR3* pPos);
};

END

#endif // Pipeline_h__
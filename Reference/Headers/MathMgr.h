/*!
* \file MathMgr.h
* \date 2017/05/01 15:53
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

#ifndef MathMgr_h__
#define MathMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CMathMgr
{
public:
	CMathMgr(void);
	~CMathMgr(void);

public:
	static void MyRotationX(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotationY(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotationZ(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
};

END

#endif // MathMgr_h__
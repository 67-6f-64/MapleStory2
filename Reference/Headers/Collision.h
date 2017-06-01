/*!
* \file Collision.h
* \date 2017/05/11 16:28
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

#ifndef Collision_h__
#define Collision_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollision
	: public CComponent
{
protected:
	CCollision(void);

public:
	virtual ~CCollision(void);

public:
	virtual CCollision* Clone(void) PURE;

protected:
	void Release(void);

protected:
	WORD*		m_pwRefCnt;

};

END

#endif // Collision_h__
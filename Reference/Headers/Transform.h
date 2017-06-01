/*!
* \file Transform.h
* \date 2017/05/01 16:11
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

#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform
	: public CComponent
{
private:
	explicit CTransform(const D3DXVECTOR3& vLook);

public:
	virtual ~CTransform(void);

public:
	virtual void Update(void);

public:
	static CTransform* Create(const D3DXVECTOR3& vLook);

public:
	float			m_fAnlge[ANGLE_END];
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vScale;
	D3DXMATRIX		m_matWorld;
};

END

#endif // Transform_h__
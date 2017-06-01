/*!
* \file Resources.h
* \date 2017/04/26 16:16
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

#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources
	: public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CResources(void);

public:
	virtual CResources* CloneResource(void) PURE;

protected:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	WORD*		m_pwRefCnt;
};

END

#endif // Resources_h__
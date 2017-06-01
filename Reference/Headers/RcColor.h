/*!
* \file RcColor.h
* \date 2017/04/26 16:40
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

#ifndef RcColor_h__
#define RcColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcColor
	: public CVIBuffer
{
private:
	explicit CRcColor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRcColor(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CRcColor* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // RcColor_h__
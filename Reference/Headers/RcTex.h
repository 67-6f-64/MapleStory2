/*!
* \file RcTex.h
* \date 2017/04/28 16:55
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

#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex
	: public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRcTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // RcTex_h__
/*!
* \file TriColor.h
* \date 2017/04/26 16:32
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

#ifndef TriColor_h__
#define TriColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriColor
	: public CVIBuffer
{
private:
	explicit CTriColor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTriColor(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CTriColor* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual void Render(void);
};

END


#endif // TriColor_h__
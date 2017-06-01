#ifndef CCubeColor_h__
#define CCubeColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor
	: public CVIBuffer
{
private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubeColor(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif
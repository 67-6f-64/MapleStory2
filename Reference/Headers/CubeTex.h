
#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex
	: public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubeTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // CubeTex_h__
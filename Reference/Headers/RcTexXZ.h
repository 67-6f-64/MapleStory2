
#ifndef RcTexXZ_h__
#define RcTexXZ_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTexXZ
	: public CVIBuffer
{
private:
	explicit CRcTexXZ(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRcTexXZ(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CRcTexXZ* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // RcTexXZ_h__
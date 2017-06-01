#ifndef SkyCubeTex_h__
#define SkyCubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSkyCubeTex :
	public Engine::CVIBuffer
{
private:
	explicit CSkyCubeTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkyCubeTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	static CSkyCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif

#ifndef WallXYTex_h__
#define WallXYTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CWallXYTex
	: public CVIBuffer
{
private:
	explicit CWallXYTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWallXYTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal);

public:
	static CWallXYTex* Create(LPDIRECT3DDEVICE9 pDevice
		, const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal);
};

END

#endif // WallXYTex_h__
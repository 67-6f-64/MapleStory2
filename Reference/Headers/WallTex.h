/*!
* \file WallTex.h
* \date 2017/05/02 15:20
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

#ifndef WallTex_h__
#define WallTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CWallTex
	: public CVIBuffer
{
private:
	explicit CWallTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWallTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal);

public:
	static CWallTex* Create(LPDIRECT3DDEVICE9 pDevice
		, const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal);
};

END

#endif // WallTex_h__
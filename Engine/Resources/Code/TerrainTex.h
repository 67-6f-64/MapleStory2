/*!
* \file TerrainTex.h
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

#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex
	: public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTerrainTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv, const D3DXVECTOR3 vNormal);

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pDevice
		, const WORD& wCntX, const WORD& wCntY, const WORD& wCntZ, const WORD& wItv
		, const D3DXVECTOR3 vNormal);
};

END

#endif // TerrainTex_h__
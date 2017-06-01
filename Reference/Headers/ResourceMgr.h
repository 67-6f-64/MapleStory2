/*!
* \file ResourceMgr.h
* \date 2017/04/26 15:33
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

#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CResources;
class CComponent;

class ENGINE_DLL CResourceMgr
{
public:
	DECLARE_SINGLETON(CResourceMgr)

private:
	CResourceMgr(void);
	~CResourceMgr(void);

public:
	CComponent* CloneResource(RESOURCETYPE eResourceID, const wstring& wstrResourceKey);

public:
	const CResources* GetBuffer(RESOURCETYPE eResourceID, const wstring& wstrResourceKey);

public:
	HRESULT AddBuffer(LPDIRECT3DDEVICE9 pDevice
		, RESOURCETYPE eResourceID
		, BUFFERTYPE eBufferID
		, const wstring wstrResourceKey
		, const WORD& wCntX = 0, const WORD& wCntY = 0, const WORD& wCntZ = 0, const WORD& wItv = 1
		, const D3DXVECTOR3 vNormal = D3DXVECTOR3(0.f,0.f,0.f));

	HRESULT	AddTexture(LPDIRECT3DDEVICE9 pDevice
		, RESOURCETYPE eResourceID
		, TEXTURETYPE eTextureID
		, const wstring wstrResourceKey
		, const wstring& wstrFilePath
		, const WORD& wCnt);

public:
	void ResetDynamic(void);

private:
	void Release(void);


private:
	typedef map<wstring, CResources*>		MAPRESOURCE;
	MAPRESOURCE		m_mapResource[RESOURCE_END];
};

END

#endif // ResourceMgr_h__
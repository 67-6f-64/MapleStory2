#include "ResourceMgr.h"
#include "TriColor.h"
#include "RcColor.h"
#include "Texture.h"
#include "RcTex.h"
#include "RcTexXZ.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "WallTex.h"
#include "WallXYTex.h"
#include "SkyCubeTex.h"
#include "CubeColor.h"

IMPLEMENT_SINGLETON(Engine::CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
{

}

Engine::CResourceMgr::~CResourceMgr(void)
{
	Release();
}

HRESULT Engine::CResourceMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice
	, RESOURCETYPE eResourceID
	, BUFFERTYPE eBufferID
	, const wstring wstrResourceKey
	, const WORD& wCntX /*= 0*/
	, const WORD& wCntY /*= 0*/
	, const WORD& wCntZ /*= 0*/
	, const WORD& wItv /*= 1*/
	, const D3DXVECTOR3 vNormal /*= D3DXVECTOR3(0.f, 0.f, 0.f)*/)
{
	MAPRESOURCE::iterator	iter = m_mapResource[eResourceID].find(wstrResourceKey);
	if (iter != m_mapResource[eResourceID].end())
	{
		MSG_BOX(wstrResourceKey.c_str());
		return S_OK;
	}

	CResources* pResource = NULL;
	switch (eBufferID)
	{
	case BUFFER_TRICOLOR:
		pResource = CTriColor::Create(pDevice);
		break;

	case BUFFER_RCCOLOR:
		pResource = CRcColor::Create(pDevice);
		break;

	case BUFFER_RCTEX:
		pResource = CRcTex::Create(pDevice);
		break;

	case BUFFER_RCTEXXZ:
		pResource = CRcTexXZ::Create(pDevice);
		break;

	case BUFFER_TERRAINTEX:
		pResource = CTerrainTex::Create(pDevice, wCntX, wCntY, wCntZ, wItv, vNormal);
		break;

	case BUFFER_WALLTEX:
		pResource = CWallTex::Create(pDevice, wCntX, wCntY, wCntZ, wItv, vNormal);
		break;
			
	case BUFFER_CUBETEX:
		pResource = CCubeTex::Create(pDevice);
		break;

	case BUFFER_CUBECOLOR:
		pResource = CCubeColor::Create(pDevice);
		break;

	case BUFFER_SKYCUBETEX:
		pResource = CSkyCubeTex::Create(pDevice);
		break;

	case BUFFER_WALLXYTEX:
		pResource = CWallXYTex::Create(pDevice, wCntX, wCntY, wCntZ, wItv, vNormal);
		break;
	}
	NULL_CHECK_RETURN(pResource, E_FAIL);
	m_mapResource[eResourceID].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;
}

HRESULT Engine::CResourceMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice
	, RESOURCETYPE eResourceID
	, TEXTURETYPE eTextureID
	, const wstring wstrResourceKey
	, const wstring& wstrFilePath
	, const WORD& wCnt)
{
	MAPRESOURCE::iterator	iter = m_mapResource[eResourceID].find(wstrResourceKey);
	if (iter != m_mapResource[eResourceID].end())
		return E_FAIL;

	CResources* pResource = CTexture::Create(pDevice, eTextureID, wstrFilePath, wCnt);
	NULL_CHECK_RETURN_MSG(pResource, E_FAIL, wstrFilePath.c_str());

	m_mapResource[eResourceID].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));
	return S_OK;
}

void Engine::CResourceMgr::Release(void)
{
	for (int i = 0; i < RESOURCE_END; ++i)
	{
		for_each(m_mapResource[i].begin(), m_mapResource[i].end(), CDeleteMap());
		m_mapResource[i].clear();
	}
}

const Engine::CResources* Engine::CResourceMgr::GetBuffer(RESOURCETYPE eResourceID, const wstring& wstrResourceKey)
{
	MAPRESOURCE::iterator	iter = m_mapResource[eResourceID].find(wstrResourceKey);
	if (iter == m_mapResource[eResourceID].end())
		return NULL;

	return iter->second;
}

Engine::CComponent* Engine::CResourceMgr::CloneResource(RESOURCETYPE eResourceID, const wstring& wstrResourceKey)
{
	MAPRESOURCE::iterator	iter = m_mapResource[eResourceID].find(wstrResourceKey);
	if (iter == m_mapResource[eResourceID].end())
	{
		wstring	wstrErr = wstrResourceKey + L"리소스 복사 실패";
		MSG_BOX(wstrErr.c_str());
		return NULL;
	}
	return iter->second->CloneResource();
}

void Engine::CResourceMgr::ResetDynamic(void)
{
	for_each(m_mapResource[RESOURCE_DYNAMIC].begin(), m_mapResource[RESOURCE_DYNAMIC].end(), CDeleteMap());
	m_mapResource[RESOURCE_DYNAMIC].clear();
}

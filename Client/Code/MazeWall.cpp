#include "stdafx.h"
#include "MazeWall.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

CMazeWall::CMazeWall(LPDIRECT3DDEVICE9 pDevice, WORD wDir)
: Engine::CGameObject(pDevice)
, m_pTexture(NULL)
, m_wDirection(wDir)
{
	ZeroMemory(m_pBuffer, sizeof(m_pBuffer));
}


CMazeWall::~CMazeWall()
{
	Release();
}

HRESULT CMazeWall::Initialize()
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CMazeWall::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CMazeWall::Render(void)
{
	//////// 재질 //////////
	D3DMATERIAL9 mtrl;

	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.f;

	m_pDevice->SetMaterial(&mtrl);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00606060);

	// 정점방향에 따라 빛의 위치가 같은 방향일 경우
	// 벽을 양방향 렌더링 해야 한다.
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer[WALL_DIR(m_wDirection)]->Render();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

CMazeWall * CMazeWall::Create(LPDIRECT3DDEVICE9 pDevice, WORD wDir/* = 0*/)
{
	CMazeWall*	pObj = new CMazeWall(pDevice, wDir);
	if (FAILED(pObj->Initialize()))
	{
		Engine::Safe_Delete(pObj);
	}

	return pObj;
}


HRESULT CMazeWall::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_LeftWallTex");
	m_pBuffer[WALL_LEFT] = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer0", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RightWallTex");
	m_pBuffer[WALL_RIGHT] = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer1", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_FrontWallTex");
	m_pBuffer[WALL_FRONT] = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer2", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_BackWallTex");
	m_pBuffer[WALL_BACK] = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer3", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_MazeWall");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CMazeWall::Release(void)
{
}

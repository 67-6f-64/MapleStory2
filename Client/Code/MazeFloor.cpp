#include "stdafx.h"
#include "MazeFloor.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

#include "Frustum.h"

CMazeFloor::CMazeFloor(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_fTime(0.f)
{

}

CMazeFloor::~CMazeFloor(void)
{
	Release();
}

HRESULT CMazeFloor::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, -120.f);
	m_pInfo->m_vScale = D3DXVECTOR3(1.1f, 1.f, 1.3f);

	m_pD3DXFont = Engine::Get_Management()->GetFont();

	RECT rc = { WINCX / 2, WINCY / 12, rc.left + 800, rc.top + 800 };
	m_FontRect = rc;

	//////// 재질 //////////
	D3DMATERIAL9 mtrl;

	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.f;

	m_pDevice->SetMaterial(&mtrl);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00606060);

	return S_OK;
}

int CMazeFloor::Update(void)
{
	Engine::CGameObject::Update();

	m_fTime += Engine::Get_TimeMgr()->GetTime();

	int a = 0, b = 0;

	a = (int)m_fTime / 60; // 분
	b = (int)m_fTime % 60; // 초

	// 01 : 35 이런식으로 출력
	wsprintf(m_szTime, L"%02d : %02d", a, b);

	return 0;
}

void CMazeFloor::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pD3DXFont->DrawTextW(nullptr, m_szTime, lstrlen(m_szTime),
		&m_FontRect, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 1);
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

CMazeFloor* CMazeFloor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMazeFloor*		pLogo = new CMazeFloor(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CMazeFloor::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_TerrainTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Floor");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CMazeFloor::Release(void)
{
}

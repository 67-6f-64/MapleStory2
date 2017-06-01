#include "Renderer.h"
#include "Scene.h"

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pScene(NULL)
	, m_pD3DXFont(NULL)
	, m_fTime(0.f)
	, m_iFrameCnt(0)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
}

Engine::CRenderer::~CRenderer(void)
{
	Release();
}

void Engine::CRenderer::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}

void Engine::CRenderer::Render(const float fTime)
{
	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(0, 0, 0)
		, 1.f, 0);
	m_pDevice->BeginScene();

	NULL_CHECK(m_pScene);
	m_pScene->Render();

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer*	pRenderer = new CRenderer(pDevice);
	if (FAILED(pRenderer->InitRenderer()))
	{
		Safe_Delete(pRenderer);
	}
	return pRenderer;
}

HRESULT Engine::CRenderer::InitRenderer(void)
{
	D3DXFONT_DESC		hFont;
	ZeroMemory(&hFont, sizeof(D3DXFONT_DESC));
	hFont.Width = 10;
	hFont.Height = 15;
	hFont.Weight = FW_NORMAL;
	lstrcpy(hFont.FaceName, L"±¼¸²");
	hFont.CharSet = HANGEUL_CHARSET;

	HRESULT hr = D3DXCreateFontIndirect(m_pDevice, &hFont, &m_pD3DXFont);
	FAILED_CHECK(hr);

	return S_OK;
}

void Engine::CRenderer::Release(void)
{
	Safe_Release(m_pD3DXFont);
}


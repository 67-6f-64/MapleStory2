#include "Management.h"
#include "Scene.h"
#include "Renderer.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
	: m_pDevice(NULL)
	, m_pScene(NULL)
	, m_pRenderer(NULL)
{
}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
	m_pRenderer = CRenderer::Create(pDevice);
	return S_OK;
}

void Engine::CManagement::Update(void)
{
	NULL_CHECK(m_pScene);
	m_pScene->Update();
}

void Engine::CManagement::Render(const float fTime)
{
	NULL_CHECK(m_pRenderer);
	m_pRenderer->Render(fTime);
}

void Engine::CManagement::Release(void)
{
	Safe_Delete(m_pRenderer);
	Safe_Delete(m_pScene);
}

const Engine::CComponent* Engine::CManagement::GetComponent(WORD LayerID, const wstring& wstrObjKey, const wstring& wstrComponentKey)
{
	return m_pScene->GetComponent(LayerID, wstrObjKey, wstrComponentKey);
}
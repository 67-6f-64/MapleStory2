#include "Scene.h"
#include "Layer.h"

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_bSceneChange(false)
, m_wSceneType(0)
, m_pbSceneFlagArr(nullptr)
, m_iPlayerCreateCnt(0)
, m_bSceneComplete(false)
{
}

Engine::CScene::~CScene(void)
{
	Release();
}

HRESULT Engine::CScene::InitScene(void)
{
	return S_OK;
}

void Engine::CScene::Update(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}

void Engine::CScene::Render(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}

void Engine::CScene::Release(void)
{
	Safe_Delete_Array(m_pbSceneFlagArr);
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

const Engine::CComponent* Engine::CScene::GetComponent(WORD LayerID, const wstring& wstrObjKey, const wstring& wstrComponentKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);
	if (iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetComponent(wstrObjKey, wstrComponentKey);
}

void Engine::CScene::SceneChange(WORD wType)
{
	m_bSceneChange = true;
	m_wSceneType = wType;
}

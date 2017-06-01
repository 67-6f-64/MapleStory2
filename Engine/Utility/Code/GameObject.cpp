#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	,m_pInfo(nullptr)
	, m_iServerIndex(-1)
	, m_bDeleteFlag(false)
{

}

Engine::CGameObject::~CGameObject(void)
{
	Release();
}

int Engine::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator iter_end = m_mapComponent.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}

	return 0;
}

void Engine::CGameObject::Render(void)
{

}

void Engine::CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();
}

const Engine::CComponent* Engine::CGameObject::GetComponent(const wstring& wstrComponentKey)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.find(wstrComponentKey);
	if (iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

void Engine::CGameObject::SetPos(D3DXVECTOR3 vecPos)
{
	m_pInfo->m_vPos = vecPos;
}


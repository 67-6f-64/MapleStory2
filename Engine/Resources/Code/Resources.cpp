#include "Resources.h"

Engine::CResources::CResources(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
	, m_pwRefCnt(new WORD(0))
{

}

Engine::CResources::~CResources(void)
{
	
}

void Engine::CResources::Release(void)
{
	Safe_Delete(m_pwRefCnt);
}


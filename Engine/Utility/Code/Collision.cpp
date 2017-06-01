#include "Collision.h"

Engine::CCollision::CCollision(void)
	: m_pwRefCnt(new WORD(0))
{

}

Engine::CCollision::~CCollision(void)
{

}

void Engine::CCollision::Release(void)
{
	Safe_Delete(m_pwRefCnt);
}


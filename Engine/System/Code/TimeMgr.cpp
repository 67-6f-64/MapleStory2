#include "TimeMgr.h"

IMPLEMENT_SINGLETON(Engine::CTimeMgr)

Engine::CTimeMgr::CTimeMgr(void)
: m_fTime(0.f)
{

}

Engine::CTimeMgr::~CTimeMgr(void)
{
}

void Engine::CTimeMgr::InitTime(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);

	QueryPerformanceFrequency(&m_CpuTick);
}

void Engine::CTimeMgr::SetTime(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;

	if (0.1f < m_fTime)
		m_fTime = 0.f;
}

float Engine::CTimeMgr::GetTime(void)
{
	return m_fTime;
}
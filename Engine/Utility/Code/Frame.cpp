#include "Frame.h"
#include "Export_Function.h"

Engine::CFrame::CFrame(const FRAME tFrame)
	:m_tFrame(tFrame)
{
}

Engine::CFrame::~CFrame(void)
{
	int i = 0;
}

void Engine::CFrame::Update(void)
{
	m_tFrame.fFrame += Get_TimeMgr()->GetTime() * m_tFrame.fCount;

	if (m_tFrame.fFrame >= m_tFrame.fMax)
		m_tFrame.fFrame = 0.f;
}

Engine::CFrame* Engine::CFrame::Create(const FRAME tFrame)
{
	return new CFrame(tFrame);
}


#include "stdafx.h"
#include "KeyMgr.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
{
	ZeroMemory(m_bKeyDown, 256);
	ZeroMemory(m_bKeyUp, 256);

	m_Input = Engine::Get_DirectInput()->GetInstance();
}

CKeyMgr::~CKeyMgr(void)
{
}

bool CKeyMgr::StayKeyDown(BYTE nKey) // 계속해서 눌리는지
{
	if (m_Input->IsKeyPressed(nKey))
		return true;

	return false;
}

bool CKeyMgr::OnceKeyDown(BYTE nKey)
{
	if(m_Input->IsKeyPressed(nKey))
	{
		if(m_bKeyDown[nKey] == false)
		{
			m_bKeyDown[nKey] = true;
			return true;
		}
	}
	else
	{
		m_bKeyDown[nKey] = false;
	}
	return false;
}

bool CKeyMgr::OnceKeyUp(BYTE nKey)
{
	if(m_Input->IsKeyPressed(nKey))
		m_bKeyUp[nKey] = true;
	else
	{
		if(m_bKeyUp[nKey] == true)
		{
			m_bKeyUp[nKey] = false;

			return true;
		}
	}

	return false;
}


bool CKeyMgr::StayLeftDown()
{
	if (m_Input->PushLeft())
		return true;

	return false;
}

bool CKeyMgr::OnceLeftDown()
{
	if (m_Input->PushLeft())
	{
		if (m_bMouseDown[0] == false)
		{
			m_bMouseDown[0] = true;
			return true;
		}
	}
	else
	{
		m_bMouseDown[0] = false;
	}
	return false;
}

bool CKeyMgr::OnceLeftUp()
{
	if (m_Input->PushLeft())
	{
		if (m_bMouseUp[0] == false)
		{
			m_bMouseUp[0] = true;
			return true;
		}
	}
	else
	{
		m_bMouseUp[0] = false;
	}
	return false;
}

bool CKeyMgr::StayRightDown()
{
	if (m_Input->PushRight())
		return true;

	return false;
}

bool CKeyMgr::OnceRightDown()
{
	if (m_Input->PushRight())
	{
		if (m_bMouseDown[2] == false)
		{
			m_bMouseDown[2] = true;
			return true;
		}
	}
	else
	{
		m_bMouseDown[2] = false;
	}
	return false;
}

bool CKeyMgr::OnceRightUp()
{
	if (m_Input->PushRight())
	{
		if (m_bMouseUp[2] == false)
		{
			m_bMouseUp[2] = true;
			return true;
		}
	}
	else
	{
		m_bMouseUp[2] = false;
	}
	return false;
}

bool CKeyMgr::OnceKeyDownForChat(int nKey)
{
	if (GetAsyncKeyState(nKey) & 0x8000)
	{
		if (m_bKeyDown[nKey] == false)
		{
			m_bKeyDown[nKey] = true;
			return true;
		}
	}
	else
	{
		m_bKeyDown[nKey] = false;
	}
	return false;

}


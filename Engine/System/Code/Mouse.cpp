#include <iostream>
#include "Mouse.h"

IMPLEMENT_SINGLETON(Engine::CMouse)

Engine::CMouse::CMouse(void)
{
	m_vecInitialPos = D3DXVECTOR3(1600 / 2.f, 900 / 2.f, 0.f);
}

Engine::CMouse::~CMouse(void)
{
}

D3DXVECTOR3 Engine::CMouse::GetMousePos(void)
{
	static POINT ptMouse;
	// ���� ���콺 Ŀ���� ��ǥ�� ������ �Լ�.
	GetCursorPos(&ptMouse);

	// ���� ��ü â�� ���� ���콺�� ��ǥ�� �ش� ������ â ���� ���� ��ǥ�� ��ȯ���ִ� �Լ�.
	ScreenToClient(m_hWnd, &ptMouse);

	return D3DXVECTOR3((float)ptMouse.x, (float)ptMouse.y, 0.f);
}

D3DXVECTOR3 Engine::CMouse::GetMouseGap(void)
{
	D3DXVECTOR3 vecMouse = GetMousePos();
	D3DXVECTOR3 vecMouseGap = vecMouse - m_vecInitialPos;
	m_vecInitialPos = vecMouse;

	return vecMouseGap;
}

D3DXVECTOR3 Engine::CMouse::GetMouseRelativeGap(void)
{
	return GetMousePos() - m_vecInitialPos;
}

void Engine::CMouse::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
}

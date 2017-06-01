#ifndef Mouse_h__
#define Mouse_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CMouse
{
public:
	DECLARE_SINGLETON(CMouse)

private:
	CMouse(void);
public:
	~CMouse(void);

public:
	D3DXVECTOR3 GetMousePos(void); // 마우스 위치
	D3DXVECTOR3 GetMouseGap(void);

	D3DXVECTOR3 GetMouseRelativeGap(void);

public:
	void Initialize(HWND hWnd);

private:
	HWND		m_hWnd;

	D3DXVECTOR3 m_vecInitialPos;
};

END

#endif // Mouse_h__
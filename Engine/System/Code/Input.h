////////////////////////////////////////////////////////////////////////////////
// Filename: input.h 
////////////////////////////////////////////////////////////////////////////////

#ifndef _INPUTCLASS_H_ 
#define _INPUTCLASS_H_

#include "Engine_Include.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass 
//////////////////////////////////////////////////////////////////////////////// 

BEGIN(Engine)

class ENGINE_DLL CInputClass
{
public:
	DECLARE_SINGLETON(CInputClass)

public:
	CInputClass();
	~CInputClass();

public:
	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	// 키보드
	bool IsKeyPressed(BYTE key);

	// 마우스
	bool PushLeft() { return (m_mouseState.rgbButtons[0] & 0x80) ? TRUE : FALSE; }
	bool PushRight() { return (m_mouseState.rgbButtons[2] & 0x80) ? TRUE : FALSE; }

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput(); // 사용하지 않고 있음

private:
	IDirectInput8* m_directInput; 
	IDirectInputDevice8* m_keyboard; 
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256]; 
	DIMOUSESTATE m_mouseState; 
	int m_screenWidth, m_screenHeight; 
	int m_mouseX, m_mouseY;
};

END

#endif
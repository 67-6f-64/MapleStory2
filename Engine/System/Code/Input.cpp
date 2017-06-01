//////////////////////////////////////////////////////////////////////////////// 
// Filename: input.cpp 
//////////////////////////////////////////////////////////////////////////////// 

#include "Input.h"

IMPLEMENT_SINGLETON(Engine::CInputClass)

Engine::CInputClass::CInputClass()
{ 
	m_directInput = 0; m_keyboard = 0; m_mouse = 0; 
} 

Engine::CInputClass::~CInputClass()
{
	Shutdown();
} 

bool Engine::CInputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result; // Store the screen size which will be used for positioning the mouse cursor.

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen. 
	m_mouseX = 0; m_mouseY = 0;

	// Initialize the main direct input interface. 
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL); 
	if (FAILED(result))
	{
		return false;
	}

	// Ű���� �ʱ�ȭ
	// Initialize the direct input interface for the keyboard. 
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL); 
	if(FAILED(result)) 
	{
		return false; 
	} 

	// Set the data format. In this case since it is a keyboard we can use the predefined data format. 
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard); 
	if(FAILED(result)) 
	{ 
		return false; 
	}

	// Set the cooperative level of the keyboard to not share with other programs. 
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE); // ���� ���ϴ� �������
	if(FAILED(result)) 
	{ 
		return false;
	}

	// Now acquire the keyboard. 
	result = m_keyboard->Acquire(); 
	if(FAILED(result)) 
	{
		return false;
	}

	// ���콺 �ʱ�ȭ
	// Initialize the direct input interface for the mouse. 
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL); 
	if(FAILED(result)) 
	{ 
		return false;
	}
	
	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse); 
	if(FAILED(result)) 
	{ 
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs. 
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); // ���� ����� ����
	if(FAILED(result))
	{ 
		return false;
	}

	// Acquire the mouse. 
	result = m_mouse->Acquire();
	if(FAILED(result)) 
	{ 
		return false;
	} 
	
	return true; 
}

void Engine::CInputClass::Shutdown()
{ // Release the mouse. 
	if(m_mouse) 
	{ 
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0; 
	} 
	
	// Release the keyboard. 
	if(m_keyboard) 
	{ 
		m_keyboard->Unacquire(); 
		m_keyboard->Release(); 
		m_keyboard = 0; 
	}
	
	// Release the main interface to direct input. 
	if(m_directInput) 
	{ 
		m_directInput->Release(); 
		m_directInput = 0; 
	} 
	
	return; 
}

bool Engine::CInputClass::Frame()
{
	bool result; 
	
	// Read the current state of the keyboard. 
	result = ReadKeyboard(); 
	if(!result) 
	{ 
		return false; 
	} 
	
	// Read the current state of the mouse.
	result = ReadMouse(); 
	if(!result)
	{ 
		return false;
	} 
	
	// Process the changes in the mouse and keyboard.
	//ProcessInput(); 
	
	return true; 
}

bool Engine::CInputClass::ReadKeyboard()
{
	HRESULT result;
	
	// Read the keyboard device. 
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState); 
	
	if(FAILED(result)) 
	{ 
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) 
		{ 
			m_keyboard->Acquire(); 
		}
		else 
		{
			return false; 
		}
	}
	return true;
}

bool Engine::CInputClass::ReadMouse()
{
	HRESULT result; 

	// Read the mouse device. 
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState); 
	
	if(FAILED(result)) 
	{ 
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) 
		{ 
			m_mouse->Acquire(); 
		} 
		else 
		{ 
			return false; 
		} 
	} 
	return true;
}

void Engine::CInputClass::ProcessInput()
{ 
	// Update the location of the mouse cursor based on the change of the mouse location during the frame. 
	
	m_mouseX += m_mouseState.lX; 
	m_mouseY += m_mouseState.lY; 
	
	// Ensure the mouse location doesn't exceed the screen width or height. 
	if(m_mouseX < 0) 
	{ 
		m_mouseX = 0; 
	} 
	if(m_mouseY < 0) 
	{
		m_mouseY = 0;
	} 
	if(m_mouseX > m_screenWidth) 
	{ 
		m_mouseX = m_screenWidth; 
	} 
	if(m_mouseY > m_screenHeight)
	{ 
		m_mouseY = m_screenHeight;
	}

	return; 
}

bool Engine::CInputClass::IsKeyPressed(BYTE key)
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed. 

	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}
	return false;
}

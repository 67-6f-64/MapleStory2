// Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MainApp.h"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//timeBeginPeriod(1);

	CMainApp MainApp;
	MainApp.InitApp();

	while (true)
		MainApp.Update();

    return 0;
}


// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define DIRECTINPUT_VERSION 0x0800
// Windows 헤더 파일:
#include <windows.h>


// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <ctime>
#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <Iostream>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

// TSW추가 서버-클라 통신용 헤더
#include <winsock2.h>
#include <thread>
#include <future>
#include <ctime>
/////////////////////////////////



using namespace std;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)

#endif
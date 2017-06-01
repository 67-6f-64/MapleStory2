// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <iostream>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <winsock2.h>
#include <thread>
#include <future>
#include <ctime>
/////////////////////////////////

#include <d3dx9.h>
#include <d3d9.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#pragma comment(lib, "WS2_32.lib")

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)

#endif
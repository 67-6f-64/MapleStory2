// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define DIRECTINPUT_VERSION 0x0800
// Windows ��� ����:
#include <windows.h>


// C�� ��Ÿ�� ��� �����Դϴ�.
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

// TSW�߰� ����-Ŭ�� ��ſ� ���
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
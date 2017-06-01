#ifndef Engine_Include_h__
#define Engine_Include_h__

#include <map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <d3dx9.h>
#include <d3d9.h>

#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

#pragma warning(disable : 4251)

#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"

// 사운드 재생을 위한 fmod 라이브러리 추가.
#include "io.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"

#pragma comment(lib, "fmodex_vc.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;

#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#endif // Engine_Include_h__
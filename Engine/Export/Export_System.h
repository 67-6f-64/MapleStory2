#pragma once

#include "GraphicDev.h"
#include "TimeMgr.h"
#include "Mouse.h"
#include "Input.h"
#include "SoundMgr.h"

BEGIN(Engine)

inline CGraphicDev* Get_GraphicDev(void);
inline CTimeMgr* Get_TimeMgr(void);
inline CMouse* Get_Mouse(void);
inline CInputClass* Get_DirectInput(void);
inline CSoundMgr* Get_SoundMgr(void);

#include "Export_System.inl"

END
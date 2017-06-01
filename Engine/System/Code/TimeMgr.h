/*!
 * \file TimeMgr.h
 * \date 2015/05/03 18:02
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CTimeMgr
{
public:
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr(void);
	~CTimeMgr(void);

public:
	void InitTime(void);
	void SetTime(void);

public:
	float GetTime(void);

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float				m_fTime;
};

END

#endif // TimeMgr_h__
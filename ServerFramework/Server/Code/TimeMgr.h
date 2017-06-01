
#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Include.h"

class CTimeMgr
{
public:
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

#endif // TimeMgr_h__
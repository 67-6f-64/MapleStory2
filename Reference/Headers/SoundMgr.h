#pragma once
#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	CSoundMgr(void);
	~CSoundMgr(void);

public:
	void Initialize(void);
	void LoadSoundFile();
	void Play_Sound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel);
	void PlayBGM(TCHAR* pSoundKey, CHANNEL_TYPE eChannel);
	void UpdateSound(void);
	void StopSound(CHANNEL_TYPE eChannel);
	void StopSoundAll(void);
	bool IsPlaying(CHANNEL_TYPE eChannel);

private:
	FMOD_SYSTEM*				m_pSystem;
	FMOD_CHANNEL*				m_pChannel[CHANNEL_END];

	map<TCHAR*, FMOD_SOUND*>	m_MapSound;
	
};

END


CGraphicDev * Get_GraphicDev(void)
{
	return CGraphicDev::GetInstance();
}

CTimeMgr * Get_TimeMgr(void)
{
	return CTimeMgr::GetInstance();
}

CMouse* Get_Mouse(void)
{
	return CMouse::GetInstance();
}

CInputClass* Get_DirectInput(void)
{
	return CInputClass::GetInstance();
}

CSoundMgr* Get_SoundMgr(void)
{
	return CSoundMgr::GetInstance();
}

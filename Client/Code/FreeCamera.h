/*!
* \file FreeCamera.h
* \date 2017/05/10 16:25
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

#ifndef FreeCamera_h__
#define FreeCamera_h__

#include "Camera.h"



namespace Engine
{
	class CTransform;
}

class CFreeCamera
	: public Engine::CCamera
{
private:
	explicit CFreeCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CFreeCamera(void);


public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CFreeCamera* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void KeyCheck(void);
	void MouseCheck(void);

	// ³ìÈ­
	void RecordKey(void);
	void Record(void);

	// ÄÆ ¾À
	void CutScene(void);
private:
	void Release(void);

private:
	float			m_fAngle[Engine::ANGLE_END];
	float			m_fCamSpeed;
	
	D3DXVECTOR3		m_vDir;

	ID3DXFont*	 m_pD3DXFont;
	RECT		 m_FontRect;
	RECT		 m_TimeRect;
	
	bool		m_bRecording;
	bool        m_bShowCutScene;
	float		m_fRecordTime;

	TCHAR		m_szRecordState[128];
	TCHAR		m_szRecordTime[16];

	HANDLE		m_hFile;

	float		m_fTime;
};

#endif // FreeCamera_h__

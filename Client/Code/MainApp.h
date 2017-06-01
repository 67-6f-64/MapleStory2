/*!
* \file MainApp.h
* \date 2017/04/25 12:19
*
* \author 정승호
* Contact: user@company.com
*
* \brief 메인 클레스
*
* TODO: long description
*
* \note
*/

#ifndef MainApp_h__
#define MainApp_h__

#include "Include.h"

namespace Engine
{
	class CGraphicDev;
	class CVIBuffer;
	class CNetwork;
}


class CMainApp
{
private:
	CMainApp(void);

public:
	~CMainApp(void);

public:
	HRESULT InitApp(void);
	void Update(void);
	void Render(void);

public:
	static CMainApp* Create(void);

private:
	void Release(void);

private:
	Engine::CGraphicDev*		m_pGraphicDev;

private:
	LPDIRECT3DDEVICE9			m_pDevice;

	// TSW 추가
private:
	Engine::CNetwork*				m_pNet;		// 통신용
};

#endif
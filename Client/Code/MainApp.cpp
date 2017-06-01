#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "SceneSelector.h"
#include "Frustum.h"
#include "KeyMgr.h"

#include "ClientNetwork.h"

CMainApp::CMainApp(void)
: m_pGraphicDev(Engine::CGraphicDev::GetInstance())
, m_pDevice(NULL)
, m_pNet(NETMGR)
{

}

CMainApp::~CMainApp(void)
{
	Release();
}

HRESULT CMainApp::InitApp(void)
{
	HRESULT hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN
		, g_hWnd, WINCX, WINCY);

	m_pDevice = m_pGraphicDev->GetDevice();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	Engine::Get_TimeMgr()->InitTime();

	CFrustumMgr::GetInstance()->Initialize();

	// 사운드
	Engine::Get_SoundMgr()->LoadSoundFile();

	hr = Engine::Get_Management()->InitManagement(m_pDevice);
	FAILED_CHECK(hr);
	hr = Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_LOGO), m_pDevice);
	FAILED_CHECK(hr);
	Engine::Get_Mouse()->Initialize(g_hWnd);

	//TSW ADD
	if (S_OK == m_pNet->InitNetwork())
	{
		// 객체 생성을 위해서 디바이스 넘겨줌
		m_pNet->SetDevice(m_pDevice);

		std::packaged_task<void()> task(bind(&CClientNetwork::RecvData, dynamic_cast<CClientNetwork*>(m_pNet)));
		thread temp(move(task));
		temp.detach();
	}

	

	return S_OK;
}

void CMainApp::Update(void)
{
	Engine::Get_TimeMgr()->SetTime();
	Engine::Get_SoundMgr()->UpdateSound();

	Engine::Get_Management()->Update();
}

void CMainApp::Render(void)
{
	Engine::Get_Management()->Render(Engine::Get_TimeMgr()->GetTime());
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pMainApp = new CMainApp;
	if (FAILED(pMainApp->InitApp()))
	{
		delete pMainApp;
		pMainApp = nullptr;
	}
	return pMainApp;
}

void CMainApp::Release(void)
{
	// 소켓 해제
	dynamic_cast<CClientNetwork*>(m_pNet)->DestroyInstance();

	Engine::Get_Management()->DestroyInstance();
	Engine::Get_GraphicDev()->DestroyInstance();
	Engine::Get_TimeMgr()->DestroyInstance();
	Engine::Get_ResourceMgr()->DestroyInstance();
	Engine::Get_Mouse()->DestroyInstance();
	Engine::Get_InfoSubject()->DestroyInstance();
	Engine::Get_DirectInput()->DestroyInstance();
	Engine::Get_SoundMgr()->DestroyInstance();

	CFrustumMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
}
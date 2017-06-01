#include "stdafx.h"
#include "Font.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "TimeMgr.h"
#include "Texture.h"

CFont::CFont(LPDIRECT3DDEVICE9 pDevice, CHAR szPrint[128], float fTime, int iIndex)
	: Engine::CGameObject(pDevice)
	, m_pGameObject(nullptr)
	, m_fTime(fTime)
	, m_iIndex(iIndex)
{
	strcpy_s(m_szName, szPrint);
	
	RECT rc = { WINCX / 2, WINCY / 2, WINCX / 2 + 300, WINCY / 2 + 300 };
	m_FontRect = rc;
}

CFont::~CFont()
{
	Release();
}

int CFont::DrawFont(RECT rc)
{
	if (m_fTime != -1.f) // -1 이면 삭제하지 않는다.
	{
		if (m_bDeleteFlag)
			return 1;

		m_fTime -= Engine::Get_TimeMgr()->GetTime();
		if (m_fTime < 0.f)
			m_bDeleteFlag = true;
	}

	CHAR szMesseage[128];
	sprintf_s(szMesseage, "플레이어%d : %s", m_iIndex + 1, m_szName);
	switch (m_iIndex)
	{
	case 0:
		// 빨강
		m_pD3DXFont->DrawTextA(nullptr, szMesseage, strlen(szMesseage), &rc, NULL, D3DCOLOR_ARGB(255, 255, 0, 0));
		break;
	case 1:
		// 파랑
		m_pD3DXFont->DrawTextA(nullptr, szMesseage, strlen(szMesseage), &rc, NULL, D3DCOLOR_ARGB(255, 0, 0, 255));
		break;
	case 2:
		// 초록
		m_pD3DXFont->DrawTextA(nullptr, szMesseage, strlen(szMesseage), &rc, NULL, D3DCOLOR_ARGB(255, 0, 255, 0));
		break;
	case 3:
		// 노랑
		m_pD3DXFont->DrawTextA(nullptr, szMesseage, strlen(szMesseage), &rc, NULL, D3DCOLOR_ARGB(255, 255, 255, 0));
		break;

	default:
		m_pD3DXFont->DrawTextA(nullptr, szMesseage, strlen(szMesseage), &rc, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}


	return 0;
}

CFont * CFont::Create(LPDIRECT3DDEVICE9 pDevice, CHAR szPrint[32], float fTime, int iIndex)
{
	CFont*	pObj = new CFont(pDevice, szPrint, fTime, iIndex);

	if (FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CFont::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pD3DXFont = Engine::Get_Management()->GetFont();

	return S_OK;
}

HRESULT CFont::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	return S_OK;
}

void CFont::Release(void)
{
}

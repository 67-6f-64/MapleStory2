#include "stdafx.h"
#include "Chat.h"

#include "Export_Function.h"
#include "Include.h"
#include "KeyMgr.h"

CChat::CChat(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_bCharMode(false)
{
	lstrcpy(m_szTemp, L"");
}

CChat::~CChat(void)
{
	Release();
}

HRESULT CChat::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pD3DXFont = Engine::Get_Management()->GetFont();

	RECT rc = { WINCX / 2 - 200, WINCY / 1.4, WINCX / 2, WINCY / 1.4 + 50 };
	m_rcChat = rc;

	return S_OK;
}

int CChat::Update(void)
{
	Engine::CGameObject::Update();

	KeyboardInput();

	return 0;
}

void CChat::Render(void)
{
	if (!m_bCharMode)
		return;

	TCHAR szUnit[128] = L"메세지 : ";

	RECT rcChat = m_rcChat;

	m_pD3DXFont->DrawTextW(nullptr, szUnit, lstrlen(szUnit), &rcChat, NULL,
		D3DCOLOR_ARGB(255, 184, 184, 232));

	rcChat.left += 80;
	rcChat.right += 80;

	m_pD3DXFont->DrawTextW(nullptr, m_szTemp, lstrlen(m_szTemp), &rcChat, NULL,
		D3DCOLOR_ARGB(255, 184, 184, 232));
}

CChat* CChat::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CChat*		pLogo = new CChat(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CChat::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	return S_OK;
}

void CChat::Release(void)
{

}

void CChat::KeyboardInput(void)
{
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_RETURN))
	{
		if (!m_bCharMode) // 채팅 모드로 진입
		{
			m_bCharMode = true;
			return;
		}
		else // 메세지 출력
		{
			if (lstrcmp(m_szTemp, L"")) // 무언가 입력되었을 때
			{
				/*CUI* pUI = new CMessage(m_szTemp);
				pUI->Initialize();
				pUI->SetBroodType(ALLY);
				m_VecMessage.push_back(pUI);*/
				
				//CObjMgr::GetInstance()->PlaySound(L"Chat.WAV", CHANNEL_CHAT);
			}

			m_bCharMode = false;

			lstrcpy(m_szTemp, L"");
		}
	}

	if (m_bCharMode)
	{
		for (int i = 0; i < 26; ++i) // 'A' to 'Z'
		{
			if (CKeyMgr::GetInstance()->OnceKeyDownForChat(i + 65))
			{
				char charBuff[] = { char(i + 65 + 32), '\0' }; // 소문자로 변환
				TCHAR szUniCode[256] = { 0, };
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, charBuff, strlen(charBuff), szUniCode, 256);

				lstrcat(m_szTemp, szUniCode);
			}
		}

		if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_SPACE)) // SpaceBar
			lstrcat(m_szTemp, L" ");

		if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_BACKSPACE)) // BackSpace
			lstrcpyn(m_szTemp, m_szTemp, lstrlen(m_szTemp));
	}
}

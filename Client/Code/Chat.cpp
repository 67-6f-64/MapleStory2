#include "stdafx.h"
#include "Chat.h"

#include "Export_Function.h"
#include "Include.h"
#include "KeyMgr.h"
#include "Font.h"
#include "ClientNetwork.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"

CChat::CChat(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_bCharMode(false)
	, m_pTexture(nullptr)
	, m_pBuffer(nullptr)
{
	strcpy_s(m_szTemp, "");
	m_pNet = NETMGR;
}

CChat::~CChat(void)
{
	Release();
}

HRESULT CChat::Initialize(void)
{
	AddComponent();

	m_pD3DXFont = Engine::Get_Management()->GetFont();

	RECT rc = { WINCX / 2 - 20, LONG(WINCY / 1.4f) - 3, rc.left + 200, rc.top + 50 };
	m_rcChat = rc;

	m_pInfo->m_vScale = D3DXVECTOR3(220.f, 16.f, 1.f);
	m_pInfo->m_vPos = D3DXVECTOR3(WINCX / 2.f + 90.f, WINCY / 1.39f, 0.f);

	m_pInfo->m_vPos.y = -m_pInfo->m_vPos.y;
	m_pInfo->m_vPos.x -= WINCX / 2.f;
	m_pInfo->m_vPos.y += WINCY / 2.f;

	D3DXMatrixOrthoLH(&m_matOrthoProj, WINCX, WINCY, 0.0f, 1.0f);
	D3DXMatrixIdentity(&m_matIdentity);

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
	ManageMessage();

	if (!m_bCharMode)
		return;

	// UI
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000001);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 원래 값 저장
	D3DXMATRIX matView2, matProj2;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView2);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj2);

	// 직교 투영
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matIdentity);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matOrthoProj);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();

	// 원복
	m_pDevice->SetTransform(D3DTS_VIEW, &matView2);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj2);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 폰트
	m_pD3DXFont->DrawTextA(nullptr, m_szTemp, strlen(m_szTemp), &m_rcChat, NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
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

void CChat::Release(void)
{
	for (size_t i = 0; i < m_vecFont.size(); ++i)
		Engine::Safe_Delete(m_vecFont[i]);
	m_vecFont.clear();
}

HRESULT CChat::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Chat");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
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
		
		else // 서버로 메세지 보내기
		{
			if (strcmp(m_szTemp, "")) // 무언가 입력되었을 때
			{
				strncpy_s(m_szTemp, m_szTemp, 15); // 버퍼의 크기가 16이니까 최대 15개까지 가능하다.

				Engine::NETDATA tData;
				ZeroMemory(&tData, sizeof(tData));
				tData.chKey = 72;
				tData.eMessage = Engine::NETMSG_CLIENT_SEND_CHATTING;
				strcpy_s(tData.szMessage, m_szTemp);
				m_pNet->SendMsg(tData);

				//CObjMgr::GetInstance()->PlaySound(L"Chat.WAV", CHANNEL_CHAT);
			}

			m_bCharMode = false;

			strcpy_s(m_szTemp, "");
		}
	}

	if (m_bCharMode)
	{
		if (strlen(m_szTemp) > 1000)
			return;

		for (int i = 0; i < 26; ++i) // 'A' to 'Z'
		{
			if (CKeyMgr::GetInstance()->OnceKeyDownForChat(i + 65))
			{
				char charBuff[] = { char(i + 65 + 32), '\0' }; // 소문자로 변환

				strcat_s(m_szTemp, charBuff);
			}
		}
		if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_PERIOD))
			strcat_s(m_szTemp, ".");

		if (CKeyMgr::GetInstance()->StayKeyDown(DIK_LSHIFT) || CKeyMgr::GetInstance()->StayKeyDown(DIK_RSHIFT)) // ?
		{
			if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_SLASH))
			{
				strcat_s(m_szTemp, "?");
			}
		}
			
		for (int i = 0; i < 10; ++i) // '0' to '9'
		{
			if (CKeyMgr::GetInstance()->OnceKeyDownForChat(i + 48)) // 4랑 9는 이상하게 여러번 눌림 49파토?
			{
				char charBuff[] = { char(i + 48), '\0' };

				strcat_s(m_szTemp, charBuff);
			}
		}

		if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_SPACE)) // 스페이스
			strcat_s(m_szTemp, " ");

		if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_BACKSPACE)) // BackSpace
			strncpy_s(m_szTemp, m_szTemp, strlen(m_szTemp)-1);
	}
}

void CChat::ManageMessage(void)
{
	for (size_t i = 0; i < m_vecFont.size(); ++i)
	{
		RECT rc = { 20, LONG(600 - (m_vecFont.size() - 1 - i) * 20), 
					rc.left + 300, rc.top + 300 };

		if (m_vecFont[i]->DrawFont(rc) == 1) // 메세지 제거
		{
			Engine::Safe_Delete(m_vecFont[i]);
			m_vecFont.erase(m_vecFont.begin() + i);
			return;
		}
	}
}

void CChat::GetServerChat(Engine::NETDATA tData)
{
	CFont* pFont = CFont::Create(m_pDevice, tData.szMessage, 5.f, tData.iIndex);
	m_vecFont.push_back(pFont);
}

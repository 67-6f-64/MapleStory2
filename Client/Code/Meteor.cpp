#include "stdafx.h"
#include "Meteor.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Include.h"
#include "Frame.h"
#include "MeteorEffect.h"

CMeteor::CMeteor(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_pFrame(nullptr)
	, m_pEffect(nullptr)
	, m_bSound(false)
{

}

CMeteor::~CMeteor(void)
{
	Release();
}

HRESULT CMeteor::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	m_pInfo->m_vScale = D3DXVECTOR3(2.f, 5.f, 2.f);

	return S_OK;
}

int CMeteor::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	Fall();

	if (nullptr != m_pEffect)
	{
		if (1 == m_pEffect->Update())
		{
			Safe_Delete(m_pEffect);
			SetDead();
		}
	}
		
	Engine::CGameObject::Update();

	// 빌보드
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	return 0;
}

void CMeteor::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// 알파 테스트
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	m_pTexture->Render(0, DWORD(m_pFrame->m_tFrame.fFrame));
	m_pBuffer->Render();

	if (nullptr != m_pEffect)
		m_pEffect->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CMeteor* CMeteor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMeteor*		pLogo = new CMeteor(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

void CMeteor::Fall(void)
{
	m_pInfo->m_vPos.y -= 25 * Engine::Get_TimeMgr()->GetTime();
	
	if (m_pInfo->m_vPos.y <= 0.f && m_pEffect == nullptr)
	{
		m_pEffect = CMeteorEffect::Create(m_pDevice);
		m_pEffect->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, 0.1f, m_pInfo->m_vPos.z));
		
		m_bSound = true;

		if (Engine::Get_SoundMgr()->IsPlaying(Engine::CHANNEL_METEOR_FALL))
			return;
	}
}

HRESULT CMeteor::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// Transform
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	// Frame
	pCompontent = m_pFrame = Engine::CFrame::Create(Engine::FRAME(0.f, 4.f, 4.f));
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Meteor");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CMeteor::Release(void)
{
	Engine::Get_InfoSubject()->RemoveData(MESSAGE_METEOR_CREATE, m_pEffect);
	if(m_pEffect != nullptr)
		Safe_Delete(m_pEffect);
}

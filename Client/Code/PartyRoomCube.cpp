#include "stdafx.h"
#include "PartyRoomCube.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"
#include "Frustum.h"

CPartyRoomCube::CPartyRoomCube(LPDIRECT3DDEVICE9 pDevice, DWORD dwRenderImage, eCUBESEQUENCE eSequence, bool bOutside)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_dwRenderImage(dwRenderImage)
	, m_ePartyRoomCubeSequence(eSequence)
	, m_bIsOutside(bOutside)
	, m_vOrignalPos(D3DXVECTOR3(0.f, 0.f, 0.f))
	, m_iCubeIndex(0)
	, m_bIsFlyInit(false)
	, m_bIsFly(true)
	, m_iFlySeq(0)
	, m_fRenderTime(0.f)
	, m_bIsDown(false)
	, m_iAngleLock(0)
{

}

CPartyRoomCube::~CPartyRoomCube(void)
{
	Release();
}

HRESULT CPartyRoomCube::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CPartyRoomCube::Update(void)
{
	if (m_bIsFly)
	{
		DownPartyCube();
	}

	Engine::CGameObject::Update();

	return 0;
}

void CPartyRoomCube::Render(void)
{
	//if (CFrustumMgr::GetInstance()->IsSphereInFrustum(m_pInfo->m_vPos, 1) == FALSE) // 절두체 컬링
	//	return;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	if (0 != m_dwRenderImage)
		int a = 0;

	m_pTexture->Render(0, m_dwRenderImage);
	m_pBuffer->Render();
}

CPartyRoomCube* CPartyRoomCube::Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwRenderImage, eCUBESEQUENCE eSequence, bool bOutside)
{
	CPartyRoomCube*		pLogo = new CPartyRoomCube(pDevice, dwRenderImage, eSequence, bOutside);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CPartyRoomCube::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Cube");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	return S_OK;
}

void CPartyRoomCube::Release(void)
{
}

void CPartyRoomCube::DownPartyCube(void)
{
	if (!m_bIsFlyInit)
	{
		m_pInfo->m_vPos.y = m_vOrignalPos.y + 15.f;
		m_bIsFlyInit = true;
		m_bIsFly = true;
		m_iAngleLock = rand() % 3 + 1;
	}

	float fTime = Engine::Get_TimeMgr()->GetTime();

	if (m_pInfo->m_vPos.y <= m_vOrignalPos.y)
	{
		// 원래 y축보다 낮아지게 되면 Fly 정지하고 OriginalPos로 고정
		m_pInfo->m_vPos.y = m_vOrignalPos.y;
		m_pInfo->m_fAnlge[Engine::ANGLE_X] = 0.f;
		m_pInfo->m_fAnlge[Engine::ANGLE_Y] = 0.f;
		m_pInfo->m_fAnlge[Engine::ANGLE_Z] = 0.f;

		m_bIsFly = false;
	}

	m_fRenderTime -= fTime;

	if (m_fRenderTime < 0.f)
	{
		m_fRenderTime = 0.01f;
		if (m_iFlySeq == m_iCubeIndex)
			m_bIsDown = true;

		++m_iFlySeq;		// <- 인덱스 순서대로 날리는거
	}

	if (m_bIsDown && m_bIsFly)
	{
		m_pInfo->m_vPos.y -= 1000.f * fTime;

		switch (m_iAngleLock)		// 자전
		{
		case 1:	// X
			m_pInfo->m_fAnlge[Engine::ANGLE_X] += 0.2f;
			break;
		case 2:	// Y
			m_pInfo->m_fAnlge[Engine::ANGLE_Y] += 0.2f;
			break;
		case 3:	// Z
			m_pInfo->m_fAnlge[Engine::ANGLE_Z] += 0.2f;
			break;
		default:
			break;
		}
	}

}

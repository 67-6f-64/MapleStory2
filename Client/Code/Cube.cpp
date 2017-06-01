#include "stdafx.h"
#include "Cube.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"
#include "Frustum.h"

#include "KeyMgr.h"

CCube::CCube(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
	, m_dwRenderImage(0)
	, m_eCubeSequence(CUBE_END)
	, m_bIsOutside(true)
	, m_vOrignalPos(D3DXVECTOR3(0.f, 0.f, 0.f))
	, m_bIsRevolutionInit(false)
	, m_bIsRevolution(true)
	, m_fSpeed(0.f)
	, m_fAngle(0.f)
	, m_fRadius(0.f)
	, m_fRenderTime(5.f)
	, m_iSequenceFlag(0)
	, m_bIsFlyInDir(true)
	, m_iCubeIndex(0)
	, m_iAngleLock(0)
{

}

CCube::~CCube(void)
{
	Release();
}

HRESULT CCube::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CCube::Update(void)
{
	if (m_bIsRevolution)
		Revolution();
	if (!m_bIsRevolution && m_bIsFlyInDir)
		FlyInDir();

	Engine::CGameObject::Update();

	return 0;
}

void CCube::Render(void)
{
	if (CFrustumMgr::GetInstance()->IsSphereInFrustum(m_pInfo->m_vPos, 1) == FALSE) // ����ü �ø�
		return;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, m_dwRenderImage);
	m_pBuffer->Render();
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCube*		pLogo = new CCube(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CCube::AddComponent(void)
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

void CCube::Release(void)
{
}

void CCube::Revolution(void)
{
	if (!m_bIsRevolutionInit)
	{
		// ������ �ϱ� ��, m_pInfo->vPos.y�� 
		// ������ �� ������ �������� �ʱ�ȭ���ش� ( y������ ��Ʈ����)
		m_pInfo->m_vPos.y = static_cast<float>(rand() % 41 - 10);

		m_fAngle = static_cast<float>(rand() % 120);
		m_bIsRevolutionInit = true;
		m_bIsRevolution = true;
		m_iAngleLock = rand() % 3 + 1;

		m_fSpeed = static_cast<float>(rand() % 30 + 10);
	}

	// ������ - Transform�� Update���� ���� Pipe::MakeWorld~�� ��*��*�̱�����(??_) ���
	// => �׷� �ƿ� vPos�� �̿��ؼ� ���ʿ� ����ϴ� �ڵ� �����ؼ� �׷��� ���� �ϴ°� �����Ű��⵵



	float	fTime = Engine::Get_TimeMgr()->GetTime();


	m_fRadius = sqrtf(pow(m_pInfo->m_vPos.x, 2.f) + pow(m_pInfo->m_vPos.z, 2.f) + 3.5f);
	// ó���� �����鼭 ���� �ڵ�

	if (m_fRadius >= 100.f)
		m_fRadius = 100.f;
	// ������ �� �̻��� ���� �ʰ� �Ѵ�


	// ������ ��ǥ = ������(��ŭ�� ũ��� ����Ұ�����) * ���� + (���� ��ǥ[�����߽ɿ��� ��������ŭ])
	m_pInfo->m_vPos.x = m_fRadius * cosf(m_fAngle) + (0) / 2.0f * fTime;
	m_pInfo->m_vPos.z = m_fRadius * sinf(m_fAngle) + (0) / 2.0f * fTime;

	// m_fAngle = �ӵ�
	m_fAngle += (0.01f * m_fSpeed) * 3.141592f / 120.0f;


	switch (m_iAngleLock)		// ����
	{
	case 1:	// X
		m_pInfo->m_fAnlge[Engine::ANGLE_X] += 0.03f;
		break;
	case 2:	// Y
		m_pInfo->m_fAnlge[Engine::ANGLE_Y] += 0.03f;
		break;
	case 3:	// Z
		m_pInfo->m_fAnlge[Engine::ANGLE_Z] += 0.03f;
		break;
	default:
		break;
	}


	// ���� �ð����� ������ ��ġ�� ������ 
	m_fRenderTime -= fTime;

	if (m_fRenderTime < 0.f)
	{
		/*m_fRenderTime = 5.f;
		if (m_iSequenceFlag == 0 && m_eCubeSequence == CUBE_FIRST)
		FlyInDir();
		if (m_iSequenceFlag == 1 && m_eCubeSequence == CUBE_SECOND)
		FlyInDir();
		if (m_iSequenceFlag == 2 && m_eCubeSequence == CUBE_FINAL)
		FlyInDir();

		++m_iSequenceFlag;*/		// <- ���� ������� �����°�


		///////////////////////////////////////////////////////////////////////

		m_fRenderTime = 0.03f;		// 0.03�ʸ��� ����

		if (m_iSequenceFlag == m_iCubeIndex)
			FlyInDir();

		++m_iSequenceFlag;		// <- �ε��� ������� �����°�

	}
}

void CCube::FlyInDir(void)
{

	m_bIsRevolution = false;

	float	fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_vDir = m_vOrignalPos - m_pInfo->m_vPos;
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	switch (m_iAngleLock)		// ����
	{
	case 1:	// X
		m_pInfo->m_fAnlge[Engine::ANGLE_X] += 0.03f;
		break;
	case 2:	// Y
		m_pInfo->m_fAnlge[Engine::ANGLE_Y] += 0.03f;
		break;
	case 3:	// Z
		m_pInfo->m_fAnlge[Engine::ANGLE_Z] += 0.03f;
		break;
	default:
		break;
	}

	m_pInfo->m_vPos += m_pInfo->m_vDir * (20.f * fTime);

	float fDist = sqrtf(
		pow(m_vOrignalPos.x - m_pInfo->m_vPos.x, 2) +
		pow(m_vOrignalPos.y - m_pInfo->m_vPos.y, 2));

	if (0.2 > fDist)		// ���� �Ÿ��� ���������
	{						// ���ƿ��°� ���߰�, OriginalSetPos ����.
		m_bIsFlyInDir = false;
		OriginalSetPos();
	}



}

void CCube::OriginalSetPos(void)
{
	m_pInfo->m_vPos.x = m_vOrignalPos.x;
	m_pInfo->m_vPos.z = m_vOrignalPos.z;

	m_pInfo->m_vPos.y = m_vOrignalPos.y - 4.f;

	m_pInfo->m_fAnlge[Engine::ANGLE_X] = 0.f;
	m_pInfo->m_fAnlge[Engine::ANGLE_Y] = 0.f;
	m_pInfo->m_fAnlge[Engine::ANGLE_Z] = 0.f;

	m_bIsRevolution = false;
}

#include "stdafx.h"
#include "BulletTrail.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "TimeMgr.h"
#include "Texture.h"


CBulletTrail::CBulletTrail(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pBuffer(NULL)
, m_fLiveTime(0.f)
, m_fMaxLiveTime(0.f)
{
}

CBulletTrail::~CBulletTrail(void)
{
	Release();
}

int CBulletTrail::Update(void)
{
	if (m_bDeleteFlag)
		return 1;

	float fTime = Engine::Get_TimeMgr()->GetTime();
	// �����ִ� �ð� üũ
	LiveTimeCheck(fTime);

	// ������ ȸ��
	m_pInfo->m_fAnlge[Engine::ANGLE_Y] += fTime * 10.f;
	
	Engine::CGameObject::Update();
	
	return 0;
}

void CBulletTrail::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render();
}

CBulletTrail * CBulletTrail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBulletTrail*	pObj = new CBulletTrail(pDevice);

	if (FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CBulletTrail::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);
	
	m_fMaxLiveTime = 0.3f;			// �ִ�� ������� �� �ִ� �ð� = 0.5��

	return S_OK;
}

HRESULT CBulletTrail::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// ���� ���
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	// ũ�� 20%�� ����
	m_pInfo->m_vScale *= 0.2f;
	m_fOriScale = m_pInfo->m_vScale.x;
	// ����
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_CubeTex", pCompontent));

	// �ؽ���
	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Trail");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture_Trail", pCompontent));
	
	return S_OK;
}

void CBulletTrail::Release(void)
{
}

void CBulletTrail::LiveTimeCheck(float fTime)
{
	m_fLiveTime += fTime;
	float fScale = m_fOriScale * (1.f - (m_fLiveTime / m_fMaxLiveTime));
	m_pInfo->m_vScale.x = fScale;
	m_pInfo->m_vScale.y = fScale;
	m_pInfo->m_vScale.z = fScale;

	// ũ�Ⱑ 0���� �۾����ų�, 0.5�ʰ� ������ ���ش�.
	if (0 > m_pInfo->m_vScale.x)
		SetDead();
}

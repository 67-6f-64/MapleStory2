#include "stdafx.h"
#include "Bullet.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Include.h"
#include "Frame.h"

#include "BulletTrail.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CGameObject(pDevice)
	, m_pBuffer(NULL)
	, m_pTexture(NULL)
{

}

CBullet::~CBullet(void)
{
	Release();
}

HRESULT CBullet::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);

	return S_OK;
}

int CBullet::Update(void)
{
	Engine::CGameObject::Update();

	Move();

	if (Disappear() == 1) // »ç°Å¸® ¹þ¾î³­ ÃÑ¾Ë
		return 1;

	// ºôº¸µå
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	m_pDevice->GetTransform(D3DTS_VIEW, &matBill);
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	// ÃÑ¾Ë ±ËÀû »ý¼º
	CBulletTrail* pObj = CBulletTrail::Create(m_pDevice);
	pObj->SetPos(m_pInfo->m_vPos);
	if(nullptr != pObj)
		m_listTrail.push_back(pObj);
	
	TrailUpdate();

	return 0;
}

void CBullet::Render(void)
{
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	// ¾ËÆÄ Å×½ºÆ®
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTexture->Render(0, DWORD(m_pFrame->m_tFrame.fFrame));
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	TrailRender();
}

CBullet* CBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBullet*		pLogo = new CBullet(pDevice);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}


HRESULT CBullet::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	// Transform
	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	// Frame
	pCompontent = m_pFrame = Engine::CFrame::Create(Engine::FRAME(0.f, 16.f, 16.f));
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Frame", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_Bullet2");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CBullet::Release(void)
{
	TrailRelease();
}

void CBullet::Move(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * 80.f;
}

int CBullet::Disappear(void)
{
	// ÃÑ¾Ë »ç¶óÁö´Â Á¶°Ç

	if (m_pInfo->m_vPos.x > 200)
		return 1;

	if (m_pInfo->m_vPos.z > 200)
		return 1;

	if (m_pInfo->m_vPos.x < 0)
		return 1;

	if (m_pInfo->m_vPos.z < 0)
		return 1;

	return 0;
}

void CBullet::TrailUpdate(void)
{
	list<CBulletTrail*>::iterator iter = m_listTrail.begin();
	list<CBulletTrail*>::iterator iter_end = m_listTrail.end();

	for (; iter != iter_end; )
	{
		if (1 == (*iter)->Update())
		{
			Engine::Safe_Delete(*iter);
			iter = m_listTrail.erase(iter);
		}
		else
			++iter;
	}		
}

void CBullet::TrailRender(void)
{
	list<CBulletTrail*>::iterator iter = m_listTrail.begin();
	list<CBulletTrail*>::iterator iter_end = m_listTrail.end();

	for (; iter != iter_end; ++iter)
		(*iter)->Render();
}

void CBullet::TrailRelease(void)
{
	list<CBulletTrail*>::iterator iter = m_listTrail.begin();
	list<CBulletTrail*>::iterator iter_end = m_listTrail.end();

	for (; iter != iter_end; ++iter)
		Engine::Safe_Delete(*iter);

	m_listTrail.clear();
}

void CBullet::SetData(BULLET_DATA BulletData)
{
	m_pInfo->m_vPos = BulletData.vecPos;
	m_pInfo->m_vDir = BulletData.vecDir;
}

#include "stdafx.h"
#include "SkyBox.h"

#include "Export_Function.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Include.h"
#include "Transform.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice, SKYBOX_ID eId)
	: Engine::CGameObject(pDevice)
	, m_pTexture(NULL)
	, m_eSkyBoxId(eId)
{
}

CSkyBox::~CSkyBox(void)
{
	Release();
}

HRESULT CSkyBox::Initialize(void)
{
	HRESULT		hr = NULL;

	hr = AddComponent();
	FAILED_CHECK(hr);
	
	return S_OK;
}

int CSkyBox::Update(void)
{
	Engine::CGameObject::Update();

	return 0;
}

void CSkyBox::Render(void)
{
	D3DXMATRIX matCamEye;
	m_pDevice->GetTransform(D3DTS_VIEW, &matCamEye);
	D3DXMatrixInverse(&matCamEye, NULL, &matCamEye);

	m_pInfo->m_matWorld._41 = matCamEye._41;
	m_pInfo->m_matWorld._42 = matCamEye._42;
	m_pInfo->m_matWorld._43 = matCamEye._43;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//텍스쳐 필터링.(기본값은 D3DTEXF_POINT)
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//텍스쳐 UV Address를 설정.(스카이박스 경계가 보이는걸 처리해준다.)
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pTexture->Render(0, m_eSkyBoxId);
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pDevice, SKYBOX_ID eId)
{
	CSkyBox*		pLogo = new CSkyBox(pDevice, eId);
	if (FAILED(pLogo->Initialize()))
	{
		Engine::Safe_Delete(pLogo);
	}

	return pLogo;
}

HRESULT CSkyBox::AddComponent(void)
{
	Engine::CComponent*		pCompontent = NULL;

	pCompontent = m_pInfo = Engine::CTransform::Create(g_Look);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Buffer_SkyCubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pCompontent));

	pCompontent = Engine::Get_ResourceMgr()->CloneResource(Engine::RESOURCE_STATIC, L"Texture_SkyBox");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pCompontent);
	NULL_CHECK_RETURN(pCompontent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pCompontent));

	return S_OK;
}

void CSkyBox::Release(void)
{

}

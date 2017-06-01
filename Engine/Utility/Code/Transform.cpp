#include "Transform.h"
#include "Pipeline.h"
#include "Export_Function.h"

Engine::CTransform::CTransform(const D3DXVECTOR3& vLook)
	: m_vPos(0.f, 0.f, 0.f)
	, m_vDir(0.f, 0.f, 0.f)
	, m_vScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_fAnlge, sizeof(float) * ANGLE_END);
	D3DXMatrixIdentity(&m_matWorld);
}

Engine::CTransform::~CTransform(void)
{

}

void Engine::CTransform::Update(void)
{
	CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale, m_fAnlge, &m_vPos);
}

Engine::CTransform* Engine::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}


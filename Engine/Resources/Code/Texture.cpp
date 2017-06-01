#include "Texture.h"
#include <iostream>

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::~CTexture(void)
{
	Release();
}

Engine::CResources* Engine::CTexture::CloneResource(void)
{
	CResources*		pResources = new CTexture(*this);

	++(*m_pwRefCnt);

	return pResources;
}

HRESULT Engine::CTexture::LoadTexture(TEXTURETYPE eTextureType, const wstring& wstrFilePath, const WORD& wCnt)
{
	IDirect3DBaseTexture9*		pTexture = NULL;

	if (wCnt == 0)
		return E_FAIL;

	m_vecTexture.reserve(wCnt);

	TCHAR		szFullPath[MAX_PATH] = L"";

	for (size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, wstrFilePath.c_str(), i);

		HRESULT		hr = NULL;
		switch (eTextureType)
		{
		case TEXTURE_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case TEXTURE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}
		FAILED_CHECK_MSG(hr, szFullPath);

		m_vecTexture.push_back(pTexture);
	}

	//�̹����� �������� ����
	m_dwContainerSize = m_vecTexture.size();
	return S_OK;
}

// ����ȭ�� �ؾ��Ѵ�.
// ��ü���� ���� SetTexture�� ȣ���ϴ� ����� ���������ϹǷ�
// ���� ��ü�� ���� 1ȸ��
// ���� ��ü�� �������� ���� ���� ��ü�� �����Ѵ�.
void Engine::CTexture::Render(const DWORD& dwStage, const DWORD& iIndex)
{
	// PartyRoom->PartyFall ���Խ� �� ��찡 �� �� �߻��ϴµ�, Release����� ��쿣 ������.
	if (iIndex >= m_dwContainerSize)
		return;

	if (m_pDevice->SetTexture(dwStage, m_vecTexture[iIndex]) == D3DERR_INVALIDCALL)
		return;//cout << "����" << endl;
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eTextureType
	, const wstring& wstrFilePath, const WORD& wCnt)
{
	CTexture*	pTexture = new CTexture(pGraphicDev);
	if (FAILED(pTexture->LoadTexture(eTextureType, wstrFilePath, wCnt)))
		Engine::Safe_Delete(pTexture);

	return pTexture;
}

void Engine::CTexture::Release(void)
{
	if ((*m_pwRefCnt) == 0)
	{
		DWORD		dwSize = m_vecTexture.size();
		for (DWORD i = 0; i < dwSize; ++i)
		{
			Engine::Safe_Release(m_vecTexture[i]);
		}
		m_vecTexture.clear();

		CResources::Release();
	}
	else
		--(*m_pwRefCnt);
}


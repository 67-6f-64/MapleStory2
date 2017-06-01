/*!
* \file Texture.h
* \date 2015/05/01 15:53
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture
	: public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual ~CTexture(void);

public:
	virtual CResources* CloneResource(void);

public:
	HRESULT LoadTexture(TEXTURETYPE eTextureType, const wstring& wstrFilePath, const WORD& wCnt);
	void Render(const DWORD& dwStage, const DWORD& iIndex);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, TEXTURETYPE eTextureType, const wstring& wstrFilePath, const WORD& wCnt);

private:
	void Release(void);

private:
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;
	VECTEXTURE		m_vecTexture;

private:
	DWORD		m_dwContainerSize;
};

END

#endif // Texture_h__
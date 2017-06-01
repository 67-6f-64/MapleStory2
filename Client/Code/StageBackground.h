

#ifndef StageBackground_h__
#define StageBackground_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CStageBackground
	: public Engine::CGameObject
{
private:
	explicit CStageBackground(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStageBackground(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CStageBackground* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

};

#endif // LogoBack_h__
/*!
* \file Management.h
* \date 2017/04/27 16:32
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

#ifndef Management_h__
#define Management_h__

#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CComponent;

class ENGINE_DLL CManagement
{
public:
	DECLARE_SINGLETON(CManagement)

private:
	CManagement(void);
	~CManagement(void);

public:
	const CComponent* GetComponent(WORD LayerID, const wstring& wstrObjKey, const wstring& wstrComponentKey);

	ID3DXFont* GetFont(void) { return m_pRenderer->GetFont(); }

public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void Update(void);
	void Render(const float fTime);

public:
	template <typename T>
	HRESULT SceneChange(T& Functor, LPDIRECT3DDEVICE9 pDevice);

private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

private:
	CScene*			m_pScene;
	CRenderer*		m_pRenderer;
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor, LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pScene != NULL)
		Safe_Delete(m_pScene);

	HRESULT hr = Functor(&m_pScene, pDevice);
	FAILED_CHECK(hr);

	m_pRenderer->SetScene(m_pScene);
	return S_OK;
}

END

#endif // Management_h__
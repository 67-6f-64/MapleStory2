#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CLayer;
class CComponent;
class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CScene(void);

public:
	virtual HRESULT	InitScene(void);
	virtual void	Update(void);
	virtual void	Render(void);

private:
	virtual void	Release(void);

public:
	const CComponent* GetComponent(WORD LayerID, const wstring& wstrObjKey, const wstring& wstrComponentKey);

	// TSW 추가
public:
	void SceneChange(WORD wType);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;

	// 씬 체인지를 위한 변수
protected:
	bool			m_bSceneChange;
	WORD			m_wSceneType;		// 클라이언트 씬 타입

	bool*			m_pbSceneFlagArr;		// 플래그가 씬마다 몇개가 필요할지 모르니 포인터로 선언해서 동적배열로 쓸 예정

protected:
	int				m_iPlayerCreateCnt;

	bool			m_bSceneComplete;
};

END
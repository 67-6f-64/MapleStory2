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

	// TSW �߰�
public:
	void SceneChange(WORD wType);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER		m_mapLayer;

	// �� ü������ ���� ����
protected:
	bool			m_bSceneChange;
	WORD			m_wSceneType;		// Ŭ���̾�Ʈ �� Ÿ��

	bool*			m_pbSceneFlagArr;		// �÷��װ� ������ ��� �ʿ����� �𸣴� �����ͷ� �����ؼ� �����迭�� �� ����

protected:
	int				m_iPlayerCreateCnt;

	bool			m_bSceneComplete;
};

END
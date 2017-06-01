/*!
* \file Layer.h
* \date 2017/04/27 15:54
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

#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer
{
private:
	CLayer(void);

public:
	~CLayer(void);

public:
	HRESULT AddObject(const wstring& wstrObjKey, CGameObject* pGameObject);
	HRESULT AddObjectFront(const wstring& wstrObjKey, CGameObject* pGameObject);

	// TSW Ãß°¡
public:
	list<CGameObject*>* GetObjList(wstring wstrObjKey);

public:
	void Update(void);
	void Render(void);

public:
	static CLayer* Create(void);

private:
	void Release(void);

public:
	const CComponent* GetComponent(const wstring& wstrObjKey, const wstring& wstrComponentKey);

private:
	typedef list<CGameObject*>		OBJLIST;
	typedef map<wstring, OBJLIST>	MAPOBJLIST;
	MAPOBJLIST		m_mapObjlist;
};

END

#endif // Layer_h__
/*!
* \file GameObject.h
* \date 2017/04/27 15:46
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

#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;
class CTransform;
class CNetwork;

class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGameObject(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// TSW ADD
	void SetDead(void) { m_bDeleteFlag = true; }
	bool GetDead(void) { return m_bDeleteFlag; }

private:
	void Release(void);

public:
	const CComponent* GetComponent(const wstring& wstrComponentKey);

	void SetServerIndex(int iIndex) { m_iServerIndex = iIndex; }
	const int GetServerIndex(void) { return m_iServerIndex; }

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

public:
	void SetLocalData(NETDATA tData) { m_LocalData = tData; }

public:
	Engine::CTransform* GetTransform(void) { return m_pInfo; }
	void SetPos(D3DXVECTOR3 vecPos);

protected:
	typedef map<wstring, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;

	Engine::CTransform*		m_pInfo;
	int						m_iServerIndex;

	// TSW ADD
protected:
	bool			m_bDeleteFlag;

	// 서버용 변수들
protected:
	NETDATA					m_LocalData;
	CNetwork*				m_pNet;
};

END

#endif // GameObject_h__
#ifndef Font_h__
#define Font_h__

#include "GameObject.h"

class CFont : public Engine::CGameObject
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice, CHAR szPrint[32], float fTime, int iIndex);

public:
	virtual ~CFont(void);

public:
	int DrawFont(RECT rc);

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice, CHAR szPrint[32], float fTime, int iIndex);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	CGameObject* m_pGameObject;
	CHAR		 m_szName[128];
	ID3DXFont*	 m_pD3DXFont;
	RECT		 m_FontRect;
	float		 m_fTime;  // 지속 시간
	int			 m_iIndex; // 누구의 채팅인지
};

#endif
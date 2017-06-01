

#ifndef Chat_h__
#define Chat_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CFont;
class CChat
	: public Engine::CGameObject
{
private:
	explicit CChat(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CChat(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CChat* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

public:
	void KeyboardInput(void);

private:

	ID3DXFont*		m_pD3DXFont;
	bool			m_bCharMode;
	TCHAR			m_szTemp[128];

	RECT			m_rcChat; // 엔터 눌렀을 때 보이는 곳
	RECT			m_rcMessage; // 왼쪽에 채팅 메세지 보이는 곳
};

#endif // LogoBack_h__


#ifndef Chat_h__
#define Chat_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CNetwork;
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
	void Release(void);

	HRESULT AddComponent(void);
public:
	void KeyboardInput(void);
	void ManageMessage(void);

	void GetServerChat(Engine::NETDATA tData);

private:
	ID3DXFont*		m_pD3DXFont;
	bool			m_bCharMode;
	CHAR			m_szTemp[1024];

	RECT			m_rcChat; // 엔터 눌렀을 때 보이는 곳
	RECT			m_rcMessage; // 왼쪽에 채팅 메세지 보이는 곳

	vector<CFont*>	m_vecFont;


private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	D3DXMATRIX				m_matIdentity;
	D3DXMATRIX				m_matOrthoProj;
};

#endif // LogoBack_h__
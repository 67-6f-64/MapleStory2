/*!
* \file Player.h
* \date 2017/05/01 15:35
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

#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Include.h"
#include "Engine_Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CFrame;
	class CNetwork;
}


class CFont;
class CPlayer
	: public Engine::CGameObject
{
	

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayer(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// TSW 추가
public:
	void SetClient(void) { m_bThisClient = true; }

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);
	void KeyCheck(void);

	void Jump(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture[24];
	
	PLAYER_DIR m_ePlayerDir;
	PLAYER_STATE m_ePlayerState;

	Engine::CFrame*			m_pFrame;
	int						m_iTexture;

	// 점프
	bool					m_bJump;
	float					m_fJumpSpeed;

	// 서버용 변수들
private:
	bool					m_bThisClient;

	// 이름
	TCHAR					m_szName[32];
	CFont*					m_pFont;

	 
};

#endif // Player_h__
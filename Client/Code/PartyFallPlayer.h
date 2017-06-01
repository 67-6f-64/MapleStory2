/*!
* \file PartyFallPlayer.h
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

#ifndef PartyFallPlayer_h__
#define PartyFallPlayer_h__

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
class CPartyFallPlayer
	: public Engine::CGameObject
{
	enum PartyFallPlayer_DIR {
		DIR_B, DIR_R, DIR_RB, DIR_RT, DIR_T,
		DIR_L, DIR_LB, DIR_LT,
	};
	enum PartyFallPlayer_STATE { PartyFallPlayer_STAND, PartyFallPlayer_WALK, PartyFallPlayer_JUMP };

private:
	explicit CPartyFallPlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPartyFallPlayer(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// TSW 추가
public:
	void SetClient(void) { m_bThisClient = true; }

public:
	static CPartyFallPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);
	void KeyCheck(void);

	// 서버로 플레이어 정보 넘겨주기
	void UpdateToServer(void);

	// 본인 클라이언트가 아닌 플레이어 정보 세팅
	void UpdateToOtherPlayer(void);

	// 플레이어 낙하 생태로 변경
	void SetFall(void) { m_bFalled = true; }
	void Fall(void);
	

	void SetPlayerStartPos(void);


private:
	int	m_itemIndex; // 충돌한 큐브의 인덱스

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture[24];

	PartyFallPlayer_DIR		m_ePartyFallPlayerDir;
	PartyFallPlayer_STATE	m_ePartyFallPlayerState;

	Engine::CFrame*			m_pFrame;
	int						m_iTexture;

	// 점프
	bool					m_bJump;
	float					m_fJumpSpeed;

	// 속도
	float					m_fSpeed; 

	// 서버용 변수들
private:
	bool					m_bThisClient;

	TCHAR					m_szName[32];
	CFont*					m_pFont;

	PLAYER_DATA*			m_pPlayerData;

	// 큐브 충돌
private:
	int						m_iCollideIndex;

	// 라운드
private:
	eGameState				m_eGameState;
	static int						m_iPlayerFall;

	// 플레이어가 낙하 했는지 판별
	bool					m_bFall;		// 낙하 중인지 판별
	bool					m_bFalled;		// 낙하가 끝났는지 판별
	float					m_fFallTime;
	float					m_fFallSpeed;

	float					m_fInitialFallTime;
	int						m_iTest;
};

#endif // PartyFallPlayer_h__
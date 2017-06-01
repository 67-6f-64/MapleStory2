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

	// TSW �߰�
public:
	void SetClient(void) { m_bThisClient = true; }

public:
	static CPartyFallPlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);
	void KeyCheck(void);

	// ������ �÷��̾� ���� �Ѱ��ֱ�
	void UpdateToServer(void);

	// ���� Ŭ���̾�Ʈ�� �ƴ� �÷��̾� ���� ����
	void UpdateToOtherPlayer(void);

	// �÷��̾� ���� ���·� ����
	void SetFall(void) { m_bFalled = true; }
	void Fall(void);
	

	void SetPlayerStartPos(void);


private:
	int	m_itemIndex; // �浹�� ť���� �ε���

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture[24];

	PartyFallPlayer_DIR		m_ePartyFallPlayerDir;
	PartyFallPlayer_STATE	m_ePartyFallPlayerState;

	Engine::CFrame*			m_pFrame;
	int						m_iTexture;

	// ����
	bool					m_bJump;
	float					m_fJumpSpeed;

	// �ӵ�
	float					m_fSpeed; 

	// ������ ������
private:
	bool					m_bThisClient;

	TCHAR					m_szName[32];
	CFont*					m_pFont;

	PLAYER_DATA*			m_pPlayerData;

	// ť�� �浹
private:
	int						m_iCollideIndex;

	// ����
private:
	eGameState				m_eGameState;
	static int						m_iPlayerFall;

	// �÷��̾ ���� �ߴ��� �Ǻ�
	bool					m_bFall;		// ���� ������ �Ǻ�
	bool					m_bFalled;		// ���ϰ� �������� �Ǻ�
	float					m_fFallTime;
	float					m_fFallSpeed;

	float					m_fInitialFallTime;
	int						m_iTest;
};

#endif // PartyFallPlayer_h__
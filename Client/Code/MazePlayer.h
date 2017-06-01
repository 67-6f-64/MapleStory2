#ifndef MazePlayer_h__
#define MazePlayer_h__

#include "GameObject.h"
#include "Include.h"
#include "Engine_Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CNetwork;
}

class CFont;
class CMazePlayer
	: public Engine::CGameObject
{

private:
	explicit CMazePlayer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMazePlayer(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// TSW �߰�
public:
	void SetClient(void) { m_bThisClient = true; }

public:
	static CMazePlayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

	void KeyCheck(void); // �̵�
	void MouseCheck(void); // ����

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	float					m_fSpeed;
	float					m_fWalkSoundTime;

	// ������ ������
private:
	bool					m_bThisClient;

	// �̸�
	TCHAR					m_szName[32];
	CFont*					m_pFont;

public:
	PLAYER_DIR*		GetPlayerDir(void) { return &m_ePlayerDir; }
	PLAYER_STATE*	GetPlayerState(void) { return &m_ePlayerState; }

private:
	PLAYER_DIR				m_ePlayerDir;
	PLAYER_STATE			m_ePlayerState;
};


#endif // !MazePlayer_h__



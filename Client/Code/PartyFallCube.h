

#ifndef PartyFallCube_h__
#define PartyFallCube_h__

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyFallCube
	: public Engine::CGameObject
{
private:
	explicit CPartyFallCube(LPDIRECT3DDEVICE9 pDevice, int iTexture);

public:
	virtual ~CPartyFallCube(void);

public:
	virtual int Update(void);
	virtual void Render(void);

	// 최초 위치 설정
	void SetInitialPos(D3DXVECTOR3 vecPos) { m_vecInitialPos = vecPos; }

	// 다시 원래 위치로(라운드 종료 시)
	void RollBackPos(void); 

public:
	static CPartyFallCube* Create(LPDIRECT3DDEVICE9 pDevice, int iTexture);
	
public:
	void Collide(void);
	void Fall(void);

	// 플레이어와 충돌!
	void SetCollide(void) { m_bCollide = true; }
	bool IsCollide(void) {	return m_bCollide; }

	// 낙하중인 큐브인지!
	bool bIsCubeFall(void) { return m_bFall; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	int						m_iTexture;
	bool					m_bCollide; // 플레이어와 충돌을 했는지
	bool					m_bFall; // 큐브 낙하
	float					m_fTime; // 흔들리는 시간

	float					m_fShakeSpeed; // 흔들리는 속도
	bool					m_bShakeLeft; // 왼쪽으로 흔들리는지

	D3DXVECTOR3				m_vecInitialPos; // 최초의 위치 (라운드 재시작 할 때 원래 위치로 돌아가기 위해서)
};

#endif // LogoBack_h__
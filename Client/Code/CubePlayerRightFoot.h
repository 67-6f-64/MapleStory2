#pragma once
#pragma once

#ifndef CubePlayerRightFoot_h__
#define CubePlayerRightFoot_h__

#include "GameObject.h"
#include "Enum.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CTransform;		// Cube�÷��̾��� Pos���󰡾� �Ǵϱ�
}


class CCubePlayerRightFoot
	: public Engine::CGameObject
{

private:
	explicit CCubePlayerRightFoot(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCubePlayerRightFoot(void);

public:
	int Update(void);
	virtual void Render(void);

public:
	static CCubePlayerRightFoot* Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pPlayerInfo);

public:
	void		SetPlayerState(const PLAYER_STATE* ePlayerState) { m_ePlayerState = ePlayerState; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);


private:
	void LookLikeWalking(void);			// �� ����
	void FootPosition(void);			// Dir�� ���� ���� ��ġ
	void SetPlayerTrans(const Engine::CTransform* pPlayerInfo);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	const Engine::CTransform*	m_pPlayerTransform;

	float						m_fAngle;
	bool						m_bIsUp;


	const PLAYER_STATE*		m_ePlayerState;
};

#endif // CubePlayerRightFoot_h__
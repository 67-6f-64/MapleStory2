

#ifndef Cube_h__
#define Cube_h__

#include "GameObject.h"
#include "Enum.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CCube
	: public Engine::CGameObject
{
private:
	explicit CCube(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCube(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CCube* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void SetCubeTexture(DWORD RenderImage)
	{
		m_dwRenderImage = RenderImage;
	}
	void SetCubeSequence(eCUBESEQUENCE Sequence)
	{
		m_eCubeSequence = Sequence;
	}
	void SetCubeSide(bool flag)
	{
		m_bIsOutside = flag;
	}


	// **공전을 위한 코드
	void SetCubeOrignalPos(D3DXVECTOR3 Pos)
	{
		m_vOrignalPos = Pos;
	}
	void SetCubeIndex(int i)
	{
		m_iCubeIndex = i;		// 0번부터 들어감
	}

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	void Revolution(void);		// 공전
	void FlyInDir(void);			// 방향으로 날아가는 코드
	void OriginalSetPos(void);

private:
	// **공전을 위한 코드



private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	DWORD					m_dwRenderImage;		// 큐브 텍스쳐
	eCUBESEQUENCE		m_eCubeSequence;		// 렌더 순서 Enum 
												// First, Second, Final(최종 렌더) , End
	bool						m_bIsOutside;


	// **공전을 위한 코드
private:
	bool					m_bIsRevolution;
	D3DXVECTOR3		m_vOrignalPos;

	float					m_fRenderTime;	// 날아가는 시간
	float					m_fRadius;			// 반지름
	float					m_fAngle;			// 각도
	float					m_fSpeed;			// 속도
	bool					m_bIsRevolutionInit;	// 처음 초기화
	int						m_iAngleLock;		// X,Y,Z중 하나 선택하게
	int						m_iSequenceFlag;	// 그냥 순서 올리게

	bool					m_bIsFlyInDir;		// false시 Original위치에 고정

	int						m_iCubeIndex;		// 인덱스
};

#endif // LogoBack_h__
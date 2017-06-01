

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


	// **������ ���� �ڵ�
	void SetCubeOrignalPos(D3DXVECTOR3 Pos)
	{
		m_vOrignalPos = Pos;
	}
	void SetCubeIndex(int i)
	{
		m_iCubeIndex = i;		// 0������ ��
	}

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	void Revolution(void);		// ����
	void FlyInDir(void);			// �������� ���ư��� �ڵ�
	void OriginalSetPos(void);

private:
	// **������ ���� �ڵ�



private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	DWORD					m_dwRenderImage;		// ť�� �ؽ���
	eCUBESEQUENCE		m_eCubeSequence;		// ���� ���� Enum 
												// First, Second, Final(���� ����) , End
	bool						m_bIsOutside;


	// **������ ���� �ڵ�
private:
	bool					m_bIsRevolution;
	D3DXVECTOR3		m_vOrignalPos;

	float					m_fRenderTime;	// ���ư��� �ð�
	float					m_fRadius;			// ������
	float					m_fAngle;			// ����
	float					m_fSpeed;			// �ӵ�
	bool					m_bIsRevolutionInit;	// ó�� �ʱ�ȭ
	int						m_iAngleLock;		// X,Y,Z�� �ϳ� �����ϰ�
	int						m_iSequenceFlag;	// �׳� ���� �ø���

	bool					m_bIsFlyInDir;		// false�� Original��ġ�� ����

	int						m_iCubeIndex;		// �ε���
};

#endif // LogoBack_h__
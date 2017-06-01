

#ifndef PartyRoomCube_h__
#define PartyRoomCube_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CPartyRoomCube
	: public Engine::CGameObject
{
private:
	explicit CPartyRoomCube(LPDIRECT3DDEVICE9 pDevice, DWORD dwRenderImage, eCUBESEQUENCE eSequence, bool bOutside);

public:
	virtual ~CPartyRoomCube(void);

public:
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPartyRoomCube* Create(LPDIRECT3DDEVICE9 pDevice, DWORD dwRenderImage, eCUBESEQUENCE eSequence, bool bOutside);

public:
	void SetPartyRoomCubeTexture(DWORD RenderImage)
	{
		m_dwRenderImage = RenderImage;
	}
	void SetPartyRoomCubeSequence(eCUBESEQUENCE Sequence)
	{
		m_ePartyRoomCubeSequence = Sequence;
	}


public:
	void SetPartyCubeOrignalPos(D3DXVECTOR3 Pos)
	{
		m_vOrignalPos = Pos;
	}
	void SetPartyCubeIndex(int i)
	{
		m_iCubeIndex = i;
	}

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	void DownPartyCube(void);

private:
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

	DWORD				m_dwRenderImage;	// ���� ���� Enum 
	eCUBESEQUENCE		m_ePartyRoomCubeSequence;	// First, Second, Final(���� ����) , End	
	bool				m_bIsOutside;		// �������� ������


private:
	D3DXVECTOR3		m_vOrignalPos;
	int						m_iCubeIndex;
	bool					m_bIsFlyInit;	// ó�� �ʱ�ȭ
	bool					m_bIsFly;		// Fly���ΰ�
	bool					m_bIsDown;

	int						m_iFlySeq;
	float					m_fRenderTime;	// ���ư��� �ð�
	int						m_iAngleLock;		// X,Y,Z�� �ϳ� �����ϰ�


};

#endif // LogoBack_h__
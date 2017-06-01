

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

	DWORD				m_dwRenderImage;	// 렌더 순서 Enum 
	eCUBESEQUENCE		m_ePartyRoomCubeSequence;	// First, Second, Final(최종 렌더) , End	
	bool				m_bIsOutside;		// 안쪽인지 밖인지


private:
	D3DXVECTOR3		m_vOrignalPos;
	int						m_iCubeIndex;
	bool					m_bIsFlyInit;	// 처음 초기화
	bool					m_bIsFly;		// Fly중인가
	bool					m_bIsDown;

	int						m_iFlySeq;
	float					m_fRenderTime;	// 날아가는 시간
	int						m_iAngleLock;		// X,Y,Z중 하나 선택하게


};

#endif // LogoBack_h__
#ifndef MazePosObserver_h__
#define MazePosObserver_h__

#include "Observer.h"
#include "Include.h"

class CMazeWall;
class CClientNetwork;
class CMazePosObserver
	: public Engine::CObserver
{
private:
	CMazePosObserver(void);

public:
	virtual ~CMazePosObserver(void);

public:
	virtual void Update(int message);

public:
	static CMazePosObserver* Create(void);

private:
	void WallCollitionCheck(D3DXVECTOR3* vPos);
	void LightCheck(D3DXVECTOR3 vPos);
	void FinishFlagCheck(D3DXVECTOR3 vPos);

public:
	int	GetPlayerWin(void) { return m_iPlayerWin; }

private:
	vector<CMazeWall*>			m_vecWall;
	CClientNetwork*				m_pNet;
	vector<pair<D3DXVECTOR3, int>> m_vecLight; // 위치, 조명위치

	D3DXVECTOR3					m_vBeforPlayerPos;

	LPDIRECT3DDEVICE9			m_pDevice;

	int							m_iPlayerWin;

	D3DXVECTOR3					m_vecFlag;
};

#endif // MazePosObserver_h__



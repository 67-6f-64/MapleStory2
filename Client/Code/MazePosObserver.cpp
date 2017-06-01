#include "stdafx.h"
#include "MazePosObserver.h"

#include "Export_Function.h"
#include "Include.h"
#include "transform.h"
#include "ClientNetwork.h"
#include "MazeWall.h"

CMazePosObserver::CMazePosObserver()
: m_pNet(NETMGR)
, m_vBeforPlayerPos(D3DXVECTOR3(0.f, 0.f, 0.f))
, m_iPlayerWin(-1)
, m_vecFlag(D3DXVECTOR3(102.004f, 2.6f, -113.587f))
{
	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
}

CMazePosObserver::~CMazePosObserver()
{
	for (size_t i = 0; i < m_vecWall.size(); ++i)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_PARTYMAZEWALL_CREATE, m_vecWall[i]);
	}
}

void CMazePosObserver::Update(int message)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);
	NULL_CHECK(pDatalist);

	switch (message)
	{
	case MESSAGE_PARTYMAZEWALL_CREATE:
		m_vecWall.push_back((CMazeWall*)pDatalist->back());
		break;

	case MESSAGE_PARTYMAZELIGHT_CREATE:
		m_vecLight.push_back(*(pair<D3DXVECTOR3, int>*)pDatalist->back());
		break;

	case MESSAGE_PLAYER_POSITION:
	{
		D3DXVECTOR3* PlayerData = &(*(D3DXVECTOR3*)pDatalist->front());
		WallCollitionCheck(PlayerData); 
		LightCheck(*PlayerData);
		FinishFlagCheck(*PlayerData);

		m_vBeforPlayerPos = *PlayerData;	// 플레이어의 이전 위치 갱신
		break;
	}
	}
}

CMazePosObserver * CMazePosObserver::Create(void)
{
	return new CMazePosObserver;
}

void CMazePosObserver::WallCollitionCheck(D3DXVECTOR3* vPos)
{
	float fCollDist = 1.1f;

	for (size_t i = 0; i < m_vecWall.size(); ++i)
	{	
		// 벽의 정보를 받아서
		D3DXVECTOR3 vWallPos = m_vecWall[i]->GetTransform()->m_vPos;
		WALL_DIR eDir = static_cast<WALL_DIR>(m_vecWall[i]->GetDir());

		// 벽의 방향에 따라 중점위치를 새로 잡아준다
		if (WALL_LEFT == eDir || WALL_RIGHT == eDir)
			vWallPos.z += 4.f*VTXITV;		// 벽의 가로 버퍼 정점의 갯수가 9이므로 4가 중점
		else
			vWallPos.x += 4.f*VTXITV;

		// 높이는 충돌체크에 필요 없으므로 플레이어 기준으로 세팅해준다.
		vWallPos.y = vPos->y;

		D3DXVECTOR3 vDist = vWallPos - (*vPos);
		float fDist = D3DXVec3Length(&vDist);

		// 벽을 중점기준 Y축 회전시겼을 때의 반지름 : 4.f*VTXITV
		// 플레이어 위치 기준 벽의 반지름보다 멀리 있으면 건너뛴다.
		if (5.f*VTXITV < fDist)
			continue;

		switch (eDir)
		{
		case WALL_FRONT:
		case WALL_BACK:
			if (m_vBeforPlayerPos.z < (vWallPos.z - fCollDist) && vPos->z >(vWallPos.z - fCollDist))
				vPos->z = m_vBeforPlayerPos.z;
			if (m_vBeforPlayerPos.z > (vWallPos.z + fCollDist) && vPos->z < (vWallPos.z + fCollDist))
				vPos->z = m_vBeforPlayerPos.z;
			break;
		case WALL_RIGHT:
		case WALL_LEFT:
			if (m_vBeforPlayerPos.x < (vWallPos.x - fCollDist) && vPos->x >(vWallPos.x - fCollDist))
				vPos->x = m_vBeforPlayerPos.x;
			if (m_vBeforPlayerPos.x > (vWallPos.x + fCollDist) && vPos->x < (vWallPos.x + fCollDist))
				vPos->x = m_vBeforPlayerPos.x;
			break;
		
		}
	}
}

void CMazePosObserver::LightCheck(D3DXVECTOR3 vPos)
{
	float fDistance = 0.f;
	for (UINT i = 0; i < m_vecLight.size(); ++i)
	{
		D3DXVECTOR3 vecDist = m_vecLight[i].first - vPos;
		fDistance = D3DXVec3Length(&vecDist);
		if(fDistance < 15.f)
			m_pDevice->LightEnable(m_vecLight[i].second, TRUE);
		else
			m_pDevice->LightEnable(m_vecLight[i].second, FALSE);
	}
	
	// Directional Light
	m_pDevice->LightEnable(500, TRUE);
}

void CMazePosObserver::FinishFlagCheck(D3DXVECTOR3 vPos)
{
	D3DXVECTOR3 vecDist = m_vecFlag- vPos;
	float fDistance = D3DXVec3Length(&vecDist);

	if (fDistance < 2.f)
		m_iPlayerWin = g_iClientIndex;
}

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

		m_vBeforPlayerPos = *PlayerData;	// �÷��̾��� ���� ��ġ ����
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
		// ���� ������ �޾Ƽ�
		D3DXVECTOR3 vWallPos = m_vecWall[i]->GetTransform()->m_vPos;
		WALL_DIR eDir = static_cast<WALL_DIR>(m_vecWall[i]->GetDir());

		// ���� ���⿡ ���� ������ġ�� ���� ����ش�
		if (WALL_LEFT == eDir || WALL_RIGHT == eDir)
			vWallPos.z += 4.f*VTXITV;		// ���� ���� ���� ������ ������ 9�̹Ƿ� 4�� ����
		else
			vWallPos.x += 4.f*VTXITV;

		// ���̴� �浹üũ�� �ʿ� �����Ƿ� �÷��̾� �������� �������ش�.
		vWallPos.y = vPos->y;

		D3DXVECTOR3 vDist = vWallPos - (*vPos);
		float fDist = D3DXVec3Length(&vDist);

		// ���� �������� Y�� ȸ���ð��� ���� ������ : 4.f*VTXITV
		// �÷��̾� ��ġ ���� ���� ���������� �ָ� ������ �ǳʶڴ�.
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

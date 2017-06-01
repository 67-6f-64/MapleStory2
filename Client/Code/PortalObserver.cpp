#include "stdafx.h"
#include "PortalObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"
#include "transform.h"
#include "ClientNetwork.h"
#include "Portal.h"
#include "KeyMgr.h"

CPortalObserver::CPortalObserver(void)
	:m_eSceneID(SCENE_END)
{
}

CPortalObserver::~CPortalObserver(void)
{
	for (size_t i = 0; i < m_vecPortal.size(); ++i)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_PORTAL_CREATE, m_vecPortal[i]);
	}
}

void CPortalObserver::Update(int message)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);

	NULL_CHECK(pDatalist);

	switch (message)
	{
	case MESSAGE_PORTAL_CREATE:
		m_vecPortal.push_back((CPortal*)pDatalist->back());
		break;

	case MESSAGE_PLAYER_POSITION:
		{
			D3DXVECTOR3 PlayerData = (*(D3DXVECTOR3*)pDatalist->front());
			CollideCheck(PlayerData); // �浹
			break;
		}
	}
}

CPortalObserver* CPortalObserver::Create(void)
{
	return new CPortalObserver;
}

bool CPortalObserver::CollideCheck(D3DXVECTOR3 PlayerPos)
{
	for (size_t i = 0; i < m_vecPortal.size(); ++i)
	{
		D3DXVECTOR3 vecPortal = m_vecPortal[i]->GetTransform()->m_vPos; // ��Ż ��ġ

		 // �÷��̾�� ��Ż���� �Ÿ�
		D3DXVECTOR3 vecDir = vecPortal - PlayerPos;
		float fDistance = D3DXVec3Length(&vecDir);

		// i == 0 �̸� PartyFall, 1�̸� PartyMaze, 2�� BattleGround
		if (fDistance <= 2) // �浹!
		{
			if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_UP)) // UpŰ
			{
				switch (i)
				{
				case 0:
					m_eSceneID = SCENE_PARTYFALL;
					break;

				case 1:
					m_eSceneID = SCENE_PARTYMAZE;
					break;

				case 2:
					m_eSceneID = SCENE_BATTLEGROUND;
					break;
				}
			}
		}
	}

	return false;
}
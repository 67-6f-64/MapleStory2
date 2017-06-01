#include "stdafx.h"
#include "Battle_ItemObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"
#include "transform.h"
#include "ClientNetwork.h"
#include "Portal.h"
#include "KeyMgr.h"
#include "Item.h"

CBattle_ItemObserver::CBattle_ItemObserver(void)
{
}

CBattle_ItemObserver::~CBattle_ItemObserver(void)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_BULLET_CREATE, m_vecItem[i]);
	}
	m_vecItem.clear();
}

void CBattle_ItemObserver::Update(int message)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);

	NULL_CHECK(pDatalist);

	switch (message)
	{
	case MESSAGE_ITEM_CREATE:
	{
		CItem* pItem = (CItem*)pDatalist->back();

		/*if (BulletData.eBullet == BULLET_END)
			break;*/

		m_vecItem.push_back(pItem);

		break;
	}

	// 아이템 & 플레이어 충돌
	case MESSAGE_BATTLE_PLAYER_DATA_ITEM:
	{
		auto iter_begin = pDatalist->begin();
		auto iter_end = pDatalist->end();

		for (; iter_begin != iter_end; ++iter_begin)
		{
			BATTLE_PLAYER_DATA* PlayerData = &(*((BATTLE_PLAYER_DATA*)(*iter_begin)));
			CollideCheck(PlayerData); // 충돌
		}
	}


	}
}

CBattle_ItemObserver* CBattle_ItemObserver::Create(void)
{
	return new CBattle_ItemObserver;
}

bool CBattle_ItemObserver::CollideCheck(BATTLE_PLAYER_DATA* PlayerData)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		D3DXVECTOR3 vecItem = m_vecItem[i]->GetTransform()->m_vPos; // 아이템 위치

		// 플레이어와 아이템과의 거리
		D3DXVECTOR3 vecDir = vecItem - PlayerData->vecPos;
		float fDistance = D3DXVec3Length(&vecDir);

		// 아이템중 포션!
		if (fDistance <= 1.5f) // 충돌!
		{
			//cout << "아이템 플레이어 충돌" << endl;
			m_vecItem[i]->SetDead();
			m_vecItem.erase(m_vecItem.begin() + i);

			if (g_iClientIndex == PlayerData->iServerIndex)
			{
				Engine::Get_SoundMgr()->Play_Sound(L"potion-inght.wav", Engine::CHANNEL_ITEM_DRINK);
				(*PlayerData).iLife++;
			}
				
		}
	}

	return false;
}
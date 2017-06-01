#include "stdafx.h"
#include "MapObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"
#include "transform.h"
#include "ClientNetwork.h"
#include "Portal.h"
#include "KeyMgr.h"
#include "Item.h"
#include "Meteor.h"

CMapObserver::CMapObserver(void)
{
}

CMapObserver::~CMapObserver(void)
{
	list<CMeteor*>::iterator iter_begin = m_listMeteor.begin();
	list<CMeteor*>::iterator iter_end = m_listMeteor.end();

	for (; iter_begin != iter_end; ++iter_begin)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_METEOR_CREATE, *iter_begin);
	}
	m_listMeteor.clear();
}

void CMapObserver::Update(int message)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);

	NULL_CHECK(pDatalist);

	switch (message)
	{
	case MESSAGE_METEOR_CREATE:
	{
		CMeteor* pMeteor = (CMeteor*)pDatalist->back();
		m_listMeteor.push_back(pMeteor);
		break;
	}

	// 메테오 & 플레이어 충돌
	case MESSAGE_BATTLE_PLAYER_DATA:
	{
		BATTLE_PLAYER_DATA* PlayerData = &(*(BATTLE_PLAYER_DATA*)pDatalist->front());
		MeteorCollideCheck(PlayerData); // 충돌
		break;
	}


	}
}

CMapObserver* CMapObserver::Create(void)
{
	return new CMapObserver;
}

bool CMapObserver::MeteorCollideCheck(BATTLE_PLAYER_DATA* PlayerData)
{
	list<CMeteor*>::iterator iter_begin = m_listMeteor.begin();
	list<CMeteor*>::iterator iter_end= m_listMeteor.end();

	for (; iter_begin != iter_end;)
	{
		if ((*iter_begin)->GetDead())
		{
			iter_begin = m_listMeteor.erase(iter_begin);
			continue;
		}

		D3DXVECTOR3 vecMeteor = (*iter_begin)->GetTransform()->m_vPos; // 메테오 이펙트 위치

		// 플레이어와 메테오와의 거리
		D3DXVECTOR3 vecDir = vecMeteor - PlayerData->vecPos;
		float fDistance = D3DXVec3Length(&vecDir);

		if (fDistance < 20.f) // 가까이서 메테오가 떨어졌을 때
		{
			if (TRUE == (*iter_begin)->GetSound())
			{
				Engine::Get_SoundMgr()->Play_Sound(L"메테오0.wav", Engine::CHANNEL_METEOR_FALL);
				(*iter_begin)->SetSound(); // 소리가 1번만 나게 한다.
			}
				
		}
			

		if (fDistance <= 2.f) // 충돌!
		{
			//cout << "메테오 플레이어 충돌" << endl;
			PlayerData->iLife--;
			iter_begin = m_listMeteor.erase(iter_begin);

			if (g_iClientIndex == PlayerData->iServerIndex) // 자신의 클라이언트가 맞았을 때 불타는 소리
				Engine::Get_SoundMgr()->Play_Sound(L"메테오1.wav", Engine::CHANNEL_METEOR_HIT);
		}
		else
			iter_begin++;
	}

	return false;
}
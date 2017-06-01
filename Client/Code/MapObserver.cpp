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

	// ���׿� & �÷��̾� �浹
	case MESSAGE_BATTLE_PLAYER_DATA:
	{
		BATTLE_PLAYER_DATA* PlayerData = &(*(BATTLE_PLAYER_DATA*)pDatalist->front());
		MeteorCollideCheck(PlayerData); // �浹
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

		D3DXVECTOR3 vecMeteor = (*iter_begin)->GetTransform()->m_vPos; // ���׿� ����Ʈ ��ġ

		// �÷��̾�� ���׿����� �Ÿ�
		D3DXVECTOR3 vecDir = vecMeteor - PlayerData->vecPos;
		float fDistance = D3DXVec3Length(&vecDir);

		if (fDistance < 20.f) // �����̼� ���׿��� �������� ��
		{
			if (TRUE == (*iter_begin)->GetSound())
			{
				Engine::Get_SoundMgr()->Play_Sound(L"���׿�0.wav", Engine::CHANNEL_METEOR_FALL);
				(*iter_begin)->SetSound(); // �Ҹ��� 1���� ���� �Ѵ�.
			}
				
		}
			

		if (fDistance <= 2.f) // �浹!
		{
			//cout << "���׿� �÷��̾� �浹" << endl;
			PlayerData->iLife--;
			iter_begin = m_listMeteor.erase(iter_begin);

			if (g_iClientIndex == PlayerData->iServerIndex) // �ڽ��� Ŭ���̾�Ʈ�� �¾��� �� ��Ÿ�� �Ҹ�
				Engine::Get_SoundMgr()->Play_Sound(L"���׿�1.wav", Engine::CHANNEL_METEOR_HIT);
		}
		else
			iter_begin++;
	}

	return false;
}
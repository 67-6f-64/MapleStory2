#include "stdafx.h"
#include "ItemObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"
#include "PartyFallItem.h"
#include "transform.h"
#include "ClientNetwork.h"

CItemObserver::CItemObserver(void)
	:m_pNet(NETMGR)
{
}

CItemObserver::~CItemObserver(void)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_PARTYFALLITEM_CREATE, m_vecItem[i]);
	}
}

void CItemObserver::Update(int message)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);
	
	NULL_CHECK(pDatalist);

	switch (message)
	{
	case MESSAGE_PARTYFALLITEM_CREATE:
		m_vecItem.push_back((CPartyFallItem*)pDatalist->back());
		break;

	case MESSAGE_PLAYER_DATA:
		{
			PLAYER_DATA* PlayerData = &(*(PLAYER_DATA*)pDatalist->front());
			CollideCheck(PlayerData); // �浹
			break;
		}
	}
}

CItemObserver* CItemObserver::Create(void)
{
	return new CItemObserver;
}

bool CItemObserver::CollideCheck(PLAYER_DATA* PlayerData)
{
	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
		if (m_vecItem[i]->IsCollide()) // �̹� �浹�� �������̸� ��ŵ!
			continue;

		D3DXVECTOR3 vecItemPos = m_vecItem[i]->GetTransform()->m_vPos; // ������ ��ġ

		// �÷��̾�� �����۰��� �Ÿ�
		D3DXVECTOR3 vecDir = vecItemPos - *(PlayerData->vecPos);
		float fDistance = D3DXVec3Length(&vecDir);

		if (fDistance <= 1) // �浹!
		{
			m_vecItem[i]->SetCollide(true);
			
			if (rand() % 2 == 0)
				*PlayerData->m_fSpeed = 20.f;
			else
				*PlayerData->m_fSpeed = 5.f;

			// ������ �������� �浹�Ѱ��� �˷��ش�.
			Engine::NETDATA tData;
			ZeroMemory(&tData, sizeof(Engine::NETDATA));
			tData.chKey = 72;
			tData.eMessage = Engine::NETMSG_CLIENT_PARTYFALL_ITEM;
			tData.iIndex = i;

			// ����
			m_pNet->SendMsg(tData);

			return true;
		}
	}

	return false;
}
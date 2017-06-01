#include "stdafx.h"
#include "BulletObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"
#include "transform.h"
#include "ClientNetwork.h"
#include "Portal.h"
#include "KeyMgr.h"
#include "Bullet.h"

CBulletObserver::CBulletObserver(void)
	:m_iBulletCreatePlayerIndex(g_iClientIndex)
	, m_bShowPlayerHitUI(false)
{
}

CBulletObserver::~CBulletObserver(void)
{
	for (size_t i = 0; i < m_VecBullet.size(); ++i)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_BULLET_CREATE, m_VecBullet[i]);
		Safe_Delete(m_VecBullet[i]);
	}
	m_VecBullet.clear();
}

void CBulletObserver::Update(int message)
{
	const list<void*>* pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);

	NULL_CHECK(pDatalist);

	switch (message)
	{
		// Bullet ����
	case MESSAGE_BULLET_CREATE: 
	{
		BULLET_DATA BulletData;
		BulletData.iIndex = -1;	// ����� ã�Ҵ��� �Ǻ��ϱ� ����

		list<BULLET_DATA*>::iterator iter = ((list<BULLET_DATA*>*)pDatalist)->begin();
		list<BULLET_DATA*>::iterator iter_end = ((list<BULLET_DATA*>*)pDatalist)->end();

		for (; iter != iter_end; ++iter)
			if (m_iBulletCreatePlayerIndex == (*iter)->iIndex)
				BulletData = *((*iter));

		if (BulletData.eBullet == BULLET_END || -1 == BulletData.iIndex)
			break;

		CBullet* pBullet = CBullet::Create(Engine::Get_GraphicDev()->GetDevice());
		pBullet->SetData(BulletData);
		m_VecBullet.push_back(pBullet);
		break;
	}
		
		// Bullet & �÷��̾� �浹
	case MESSAGE_BATTLE_PLAYER_DATA:
	{
		auto iter_begin = pDatalist->begin();
		auto iter_end = pDatalist->end();

		for (; iter_begin != iter_end; ++iter_begin)
		{
			BATTLE_PLAYER_DATA* PlayerData = &(*((BATTLE_PLAYER_DATA*)(*iter_begin)));
			CollideCheck(PlayerData); // �浹
		}
	}

	// �ҷ� ������ �÷��̾��� �ε������� ����
	case MESSAGE_BULLET_PLAYER_INDEX:
		m_iBulletCreatePlayerIndex = *((int*)pDatalist->front());
		break;
	}
}

CBulletObserver* CBulletObserver::Create(void)
{
	return new CBulletObserver;
}

bool CBulletObserver::CollideCheck(BATTLE_PLAYER_DATA* PlayerData)
{
	for (size_t i = 0; i < m_VecBullet.size(); ++i)
	{
		D3DXVECTOR3 vecBullet = m_VecBullet[i]->GetTransform()->m_vPos; // �ҷ� ��ġ

		// �÷��̾�� �ҷ����� �Ÿ�
		D3DXVECTOR3 vecDir = vecBullet - PlayerData->vecPos;
		float fDistance = D3DXVec3Length(&vecDir);

		if (fDistance <= 1) // �浹!
		{
			//cout << "�Ѿ� �÷��̾� �浹" << endl;

			if (g_iClientIndex == PlayerData->iServerIndex) // �ڱ� Ŭ���̾�Ʈ �÷��̾ �¾�������
			{
				if (0 < PlayerData->iLife)
				{
					PlayerData->iLife--;
					m_bShowPlayerHitUI = true; // UI ���
				}
			}
			Safe_Delete(m_VecBullet[i]);
			m_VecBullet.erase(m_VecBullet.begin() + i);
		}
	}

	return false;
}

void CBulletObserver::Update(void)
{
	int iResult = 0;

	for (size_t i = 0; i < m_VecBullet.size(); ++i)
	{
		iResult = m_VecBullet[i]->Update();

		if (iResult == 1) // �Ѿ� ����
		{
			Safe_Delete(m_VecBullet[i]);
			m_VecBullet.erase(m_VecBullet.begin() + i);
		}
	}
}

void CBulletObserver::Render(void)
{
	for (size_t i = 0; i < m_VecBullet.size(); ++i)
	{
		m_VecBullet[i]->Render();
	}
}

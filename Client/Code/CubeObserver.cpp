#include "stdafx.h"
#include "CubeObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"
#include "ClientNetwork.h"

CCubeObserver::CCubeObserver(void)
	:m_pNet(NETMGR)
{
}

CCubeObserver::~CCubeObserver(void)
{
	for (size_t i = 0; i < m_vecPartyFallCube.size(); ++i)
	{
		Engine::Get_InfoSubject()->RemoveData(MESSAGE_PARTYFALLCUBE_CREATE, m_vecPartyFallCube[i]);
	}
}

void CCubeObserver::Update(int message)
{
	const list<void*>*		pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);
	NULL_CHECK(pDatalist);

	switch (message)
	{
		// ť�� ����
	case MESSAGE_PARTYFALLCUBE_CREATE:
		m_vecPartyFallCube.push_back((CPartyFallCube*)pDatalist->back());
		break;

		// �÷��̾�� ť���� �浹
	case MESSAGE_PLAYER_POSITION:
		{
		auto iter_begin = pDatalist->begin();
		auto iter_end = pDatalist->end();

		for (; iter_begin != iter_end; ++iter_begin)
		{
			D3DXVECTOR3* pPlayerPos = &(*(D3DXVECTOR3*) *(iter_begin));

			// y���� 0�� ���� �浹üũ�ϸ� ��
			if(0 == pPlayerPos->y)
				CollideCheck(pPlayerPos); // �浹
		}
		break;

		}

	}
}

bool CCubeObserver::CollideCheck(D3DXVECTOR3* vecPos)
{
	// ���� ť��� 30X30(900)��, (0,0) ~ (1800,1800)
	// �ε��� �˻�
	int iIndex = (int)((vecPos->x + 1) / 2) + PARTYFALL_CUBECNT*(int)((vecPos->z + 1) / 2);

	// ť�� ũ��� 2x2x2, ť���� ������ 0,0,0
	if (PARTYFALL_CUBECNT * 2 - 1 < vecPos->x || -1.f > vecPos->x ||
		PARTYFALL_CUBECNT * 2 - 1 < vecPos->z || -1.f > vecPos->z)
	{
		vecPos->y -= 0.1f; // ��ܷ� ������
		return false;
	}
		
	if (iIndex < 0 || iIndex >= 900)
		return false;

	// �̹� ������ ť���� �÷��̾�� ��������.
	if (m_vecPartyFallCube[iIndex]->bIsCubeFall() == TRUE)
	{
		vecPos->y -= 0.1f;
		return false;
	}

	// ������ �ּ����� �����͸� ������ ����
	if (m_vecPartyFallCube[iIndex]->IsCollide() == TRUE) 
		return false;

	m_vecPartyFallCube[iIndex]->SetCollide();

	return true;
}


CCubeObserver* CCubeObserver::Create(void)
{
	return new CCubeObserver;
}


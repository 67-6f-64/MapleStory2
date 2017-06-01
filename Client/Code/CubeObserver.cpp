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
		// 큐브 생성
	case MESSAGE_PARTYFALLCUBE_CREATE:
		m_vecPartyFallCube.push_back((CPartyFallCube*)pDatalist->back());
		break;

		// 플레이어와 큐브의 충돌
	case MESSAGE_PLAYER_POSITION:
		{
		auto iter_begin = pDatalist->begin();
		auto iter_end = pDatalist->end();

		for (; iter_begin != iter_end; ++iter_begin)
		{
			D3DXVECTOR3* pPlayerPos = &(*(D3DXVECTOR3*) *(iter_begin));

			// y값이 0일 때만 충돌체크하면 됨
			if(0 == pPlayerPos->y)
				CollideCheck(pPlayerPos); // 충돌
		}
		break;

		}

	}
}

bool CCubeObserver::CollideCheck(D3DXVECTOR3* vecPos)
{
	// 현재 큐브는 30X30(900)개, (0,0) ~ (1800,1800)
	// 인덱스 검색
	int iIndex = (int)((vecPos->x + 1) / 2) + PARTYFALL_CUBECNT*(int)((vecPos->z + 1) / 2);

	// 큐브 크기는 2x2x2, 큐브의 중점은 0,0,0
	if (PARTYFALL_CUBECNT * 2 - 1 < vecPos->x || -1.f > vecPos->x ||
		PARTYFALL_CUBECNT * 2 - 1 < vecPos->z || -1.f > vecPos->z)
	{
		vecPos->y -= 0.1f; // 장외로 떨어짐
		return false;
	}
		
	if (iIndex < 0 || iIndex >= 900)
		return false;

	// 이미 떨어진 큐브라면 플레이어는 떨어진다.
	if (m_vecPartyFallCube[iIndex]->bIsCubeFall() == TRUE)
	{
		vecPos->y -= 0.1f;
		return false;
	}

	// 서버에 최소한의 데이터를 보내기 위함
	if (m_vecPartyFallCube[iIndex]->IsCollide() == TRUE) 
		return false;

	m_vecPartyFallCube[iIndex]->SetCollide();

	return true;
}


CCubeObserver* CCubeObserver::Create(void)
{
	return new CCubeObserver;
}


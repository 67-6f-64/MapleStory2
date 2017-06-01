#include "stdafx.h"
#include "RoundObserver.h"
#include "PartyFallCube.h"

#include "Export_Function.h"
#include "Include.h"

CRoundObserver::CRoundObserver(void)
{
}

CRoundObserver::~CRoundObserver(void)
{
	
}

void CRoundObserver::Update(int message)
{
	const list<void*>*		pDatalist = Engine::Get_InfoSubject()->GetDatalist(message);
	NULL_CHECK(pDatalist);

	switch (message)
	{
	case MESSAGE_PARTYFALLPLAYER_CREATE:
		PushPlayer();
		break;

	case MESSAGE_PLAYER_FALL:
		{
			int iIndex = (*(int*)pDatalist->front());
			if(iIndex >= 0)
				m_VecPlayerFall[iIndex] = true;
			break;
		}
		
	default:
		break;
	}
}

int CRoundObserver::IsOnlyOneSurvive(void)
{
	int iCount = 0;
	int iPlayerWin = -1;

	if (m_VecPlayerFall.size() == 1) // 혼자선 승리하지 않는다.
		return -1;

	for (size_t i = 0; i < m_VecPlayerFall.size(); ++i)
	{
		if (m_VecPlayerFall[i] == false) // 살아있다
		{
			iPlayerWin = i;
			iCount++;
		}
	}
	
	if (iCount == 1) // 1명만 살아있다
		return iPlayerWin;

	return -1;
}

CRoundObserver* CRoundObserver::Create(void)
{
	return new CRoundObserver;
}


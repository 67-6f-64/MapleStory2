/*!
* \file SceneSelector.h
* \date 2017/04/28 15:32
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Include.h"
#include "Logo.h"
#include "Stage.h"
#include "PartyRoom.h"
#include "PartyFall.h"
#include "PartyMaze.h"
#include "BattleGround.h"

class CSceneSelector
{
public:
	explicit CSceneSelector(SCENEID eSceneID)
		: m_eSceneID(eSceneID) {};
	~CSceneSelector(void) {};


public:
	HRESULT operator()(Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice)
	{
		switch (m_eSceneID)
		{
		case SCENE_LOGO:
			*ppScene = CLogo::Create(pDevice);
			break;

		case SCENE_STAGE:
			*ppScene = CStage::Create(pDevice);
			break;

		case SCENE_PARTYROOM:
			*ppScene = CPartyRoom::Create(pDevice);
			break;

		case SCENE_PARTYFALL:
			*ppScene = CPartyFall::Create(pDevice);
			break;

		case SCENE_PARTYMAZE:
			*ppScene = CPartyMaze::Create(pDevice);
			break;

		case SCENE_BATTLEGROUND:
			*ppScene = CBattleGround::Create(pDevice);
			break;
		}
		NULL_CHECK_RETURN(*ppScene, E_FAIL);
		return S_OK;
	}

private:
	SCENEID		m_eSceneID;
};

#endif // SceneSelector_h__
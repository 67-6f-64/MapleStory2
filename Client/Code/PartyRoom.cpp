#include "stdafx.h"
#include "PartyRoom.h"

#include "Include.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Layer.h"
#include "GameObject.h"
#include "KeyMgr.h"
#include "Floor.h"
#include "Player.h"
#include "StaticCamera.h"
#include "PartyRoomBackground.h"
#include "PartyRoomCube.h"
#include "chat.h"
#include "FreeCamera.h"
#include "Portal.h"
#include "PortalObserver.h"
#include "PlayerLookCamera.h"

#include "ClientNetwork.h"

CPartyRoom::CPartyRoom(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CScene(pDevice)
	, m_pNet(NETMGR)
	, m_iLightCount(0)
	, m_pPortalObserver(nullptr)
{
	m_wSceneType = 2;
}

CPartyRoom::~CPartyRoom(void)
{
	Release();
}

HRESULT CPartyRoom::Initialize(void)
{
	HRESULT		hr = NULL;

	Engine::Get_SoundMgr()->StopSoundAll();
	Engine::Get_SoundMgr()->PlayBGM(L"BGM_Ludibrium_01.mp3", Engine::CHANNEL_PARTYROOM);
	Engine::Get_SoundMgr()->Play_Sound(L"Cave_In-stephan_schutze-1369506733.wav", Engine::CHANNEL_PARTYMAZE_THUNDER);

	// 옵저버
	m_pPortalObserver = CPortalObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pPortalObserver);

	/////////////////////////////////////////////////////////
	// 네트워크에 레이어 세팅
	m_pNet->SetLayer(&m_mapLayer);
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);

	m_pbSceneFlagArr = new bool[FLAG_END];
	memset(m_pbSceneFlagArr, false, FLAG_END * sizeof(bool));
	m_pNet->SetSceneFlagArr(&m_pbSceneFlagArr);

	//cout << "- 파티룸 플래그 배열 주소: " << m_pbSceneFlagArr << endl;
	/////////////////////////////////////////////////////////

	hr = Add_Environment_Layer();	FAILED_CHECK(hr);
	hr = Add_GameLogic_Layer();		FAILED_CHECK(hr);
	hr = Add_UI_Layer();			FAILED_CHECK(hr);

	Engine::NETDATA tData;
	tData.chKey = 72;

	// 플레이어 생성 요청
	tData.eMessage = Engine::NETMSG_CLIENT_CREATE_PLAYER;
	m_pNet->SendMsg(tData);

	++m_iPlayerCreateCnt;

	// 빛
	Add_Light(D3DXVECTOR3(5.f, 5.f, 5.f));
	Add_Light(D3DXVECTOR3(45.f, 5.f, 5.f));
	Add_Light(D3DXVECTOR3(5.f, 5.f, 45.f));
	Add_Light(D3DXVECTOR3(45.f, 5.f, 45.f));
	
	return S_OK;
}

void CPartyRoom::Update(void)
{
	// 플래그에 따라 함수 실행
	FlagFuntion();

	Engine::CScene::Update();

	SCENEID eId = m_pPortalObserver->GetSceneId();

	if (eId == SCENE_PARTYFALL || CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADENTER))
	{
		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		tData.wSubMessage = (WORD)SCENE_PARTYFALL;

		m_pNet->SendMsg(tData);
	}
	else if(eId == SCENE_PARTYMAZE || CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADPLUS))
	{
		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		tData.wSubMessage = (WORD)SCENE_PARTYMAZE;

		m_pNet->SendMsg(tData);
	}
	else if (eId == SCENE_BATTLEGROUND || CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADMINUS))
	{
		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		tData.wSubMessage = (WORD)SCENE_BATTLEGROUND;

		m_pNet->SendMsg(tData);
	}

	//cout << "플레이어 수: " << m_iPlayerCreateCnt << endl;
}

void CPartyRoom::Render(void)
{
	Engine::CScene::Render();

	if (m_bSceneChange) // Render에 있는 이유 : Stage의 Update부터 시작하기 위해
	{
		//cout << "CPartyRoom - 씬 변경" << endl;
		m_pNet->SetSceneFlagArrNull();

		m_pNet->SetLayer(nullptr);
		Engine::Get_Management()->SceneChange(CSceneSelector((SCENEID)m_wSceneType), m_pDevice);
		return;
	}
}

CPartyRoom* CPartyRoom::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyRoom*	pScene = new CPartyRoom(pDevice);
	if (FAILED(pScene->Initialize()))
		Safe_Delete(pScene);

	return pScene;
}

HRESULT CPartyRoom::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_ENVIROMENT))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_ENVIROMENT];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	// 큐브
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../bin/MapData/PartyroomCubeMap.Dat",
		GENERIC_READ,
		NULL, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	int i = 0;

	while (true)
	{
		CUBEDATA* pCubeData = new CUBEDATA;

		ReadFile(hFile, pCubeData, sizeof(CUBEDATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(pCubeData);
			break;
		}

		pGameObject = CPartyRoomCube::Create(m_pDevice, pCubeData->dwTextureNum, pCubeData->eCubeSequence, pCubeData->bOutSide);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->SetPos(pCubeData->vPos);
		dynamic_cast<CPartyRoomCube*>(pGameObject)->SetPartyCubeOrignalPos(pCubeData->vPos);
		dynamic_cast<CPartyRoomCube*>(pGameObject)->SetPartyCubeIndex(i++);

		// 큐브 텍스쳐 번호에 따라 키값 일괄 적용
		TCHAR szCube[16] = {};
		wsprintf(szCube, L"Cube%d", pCubeData->dwTextureNum);

		pLayer->AddObject(szCube, pGameObject);

		Engine::Safe_Delete(pCubeData);
	}
	CloseHandle(hFile);

	pGameObject = CFloor::Create(m_pDevice);
	pGameObject->SetPos(D3DXVECTOR3(0.f, -1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Floor", pGameObject);

	pGameObject = CPartyRoomBackground::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"PartyRoomBackground", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIROMENT, pLayer));

	return S_OK;
}

HRESULT CPartyRoom::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_GAMELOGIC))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_GAMELOGIC];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	// 플레이어
	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObjectFront(L"Player", pGameObject);

	// 클라이언트 세팅
	dynamic_cast<CPlayer*>(pGameObject)->SetClient();
	// 인덱스 세팅
	pGameObject->SetServerIndex(g_iClientIndex);		// 서버 접속할 때 받아놓은 인덱스

	//cout << endl << " - " << g_iClientIndex + 1 << " 번 클라이언트" << endl << endl;

	// 포탈
	pGameObject = CPortal::Create(m_pDevice, PORTAL_PARTYROOM_TO_PARTYFALL);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(45.f, 0.f, 45.f));
	pLayer->AddObjectFront(L"Portal_To_PartyFall", pGameObject);
	
	Engine::Get_InfoSubject()->AddData(MESSAGE_PORTAL_CREATE, pGameObject);

	pGameObject = CPortal::Create(m_pDevice, PORTAL_PARTYROOM_TO_PARTYMAZE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(5.f, 0.f, 45.f));
	pLayer->AddObjectFront(L"Portal_To_PartyMaze", pGameObject);

	Engine::Get_InfoSubject()->AddData(MESSAGE_PORTAL_CREATE, pGameObject);

	pGameObject = CPortal::Create(m_pDevice, PORTAL_PARTYROOM_TO_BATTLEGROUND);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(40.f, 0.f, 10.f));
	pLayer->AddObjectFront(L"Portal_To_BattleGround", pGameObject);

	Engine::Get_InfoSubject()->AddData(MESSAGE_PORTAL_CREATE, pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CPartyRoom::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_UI))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_UI];

	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	// PlayerLook Camera
	Engine::CTransform* pTransform = (Engine::CTransform*)m_mapLayer[LAYER_GAMELOGIC]->GetComponent(L"Player", L"Transform");
	pGameObject = CPlayerLookCamera::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"PlayerLookCamera", pGameObject);

	// Free Camera
	/*pGameObject = CFreeCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"FreeCamera", pGameObject);*/

	// 채팅
	pGameObject = CChat::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Chat", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CPartyRoom::Add_Light(D3DXVECTOR3 vecPos)
{
	//////// 조명 //////////

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_POINT;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	// D3DLIGHT_DIRECTION 일 때

	//D3DXVECTOR3 vecDir = { 10.f, -2.f, 30.f };
	//D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	// 조명 위치
	light.Position = vecPos;

	// 감쇠값, 범위
	light.Attenuation0 = 0.000001f;
	light.Range = 20.f;

	m_pDevice->SetLight(m_iLightCount, &light);
	m_pDevice->LightEnable(m_iLightCount, TRUE);

	m_iLightCount++;
}

void CPartyRoom::FlagFuntion(void)
{
	if (m_pbSceneFlagArr[FLAG_PLAYER_CREATE])
	{
		PlayerCreate();		// 한번 실행 해 주었으면 플래그를 되돌린다.
		m_pbSceneFlagArr[FLAG_PLAYER_CREATE] = false;
	}

	if (m_pbSceneFlagArr[FLAG_PLAYER_UPDATE])
	{
		PlayerUpdate();
		m_pbSceneFlagArr[FLAG_PLAYER_UPDATE] = false;
	}
}

void CPartyRoom::PlayerCreate(void)
{
	// 현재 연결되어 있는 클라이언트의 갯수를 가져온다.
	const size_t iCnt = ((Engine::NETDATA*)(m_pNet->GetMapNetData(WORD(FLAG_PLAYER_CREATE))))->iIndex;

	auto iter = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
	auto iter_end = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->end();

	if (iCnt <= m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->size())
		return;

	// 클라이언트 수만큼 벡터 만들어서 판별
	vector<bool> vPlayer;
	for (size_t i = 0; i < iCnt; ++i)
		vPlayer.push_back(false);

	// 현재 만들어져 있는 플레이어는 true로 변경시켜서 안만들어져있는 놈만 만들 예정
	for (; iter != iter_end; ++iter)
		vPlayer[(*iter)->GetServerIndex()] = true;

	//cout << endl << "-------------------------------" << endl;
	//for (size_t i = 0; i < vPlayer.size(); ++i)
	//{
	//	cout << "[ " << i + 1 << " | " << vPlayer[i] << "] ";
	//}
	//cout << endl;

	for (size_t i = 0; i < vPlayer.size(); ++i)
	{
		// 플레이어가 안만들어진 인덱스만 생성 : false인 인덱스
		if (!vPlayer[i])
		{
			Engine::CGameObject*	pObj = CPlayer::Create(m_pDevice);
			pObj->SetServerIndex(i);
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(_T("Player"), pObj);

			//cout << i + 1 << "번 플레이어 생성" << endl;
		}
	}

	++m_iPlayerCreateCnt;
}

void CPartyRoom::PlayerUpdate(void)
{
	MAPPLAYERDATA::iterator iter = ((MAPPLAYERDATA*)(m_pNet->GetMapNetData(WORD(FLAG_PLAYER_UPDATE))))->begin();
	MAPPLAYERDATA::iterator iter_end = ((MAPPLAYERDATA*)(m_pNet->GetMapNetData(WORD(FLAG_PLAYER_UPDATE))))->end();

	auto playerIter = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
	auto playerIter_end = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->end();

	for (; iter != iter_end; ++iter)
	{
		playerIter = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
		for (; playerIter != playerIter_end; ++playerIter)
		{
			if ((*playerIter)->GetServerIndex() == (*iter).second->iIndex)
			{
				(*playerIter)->SetLocalData(*(*iter).second);
			}
		}
	}
}

void CPartyRoom::Release(void)
{
	for (int i = 0; i < m_iLightCount; ++i)
		m_pDevice->LightEnable(i, FALSE);

	Engine::Get_InfoSubject()->UnSubscribe(m_pPortalObserver);
	Safe_Delete(m_pPortalObserver);

	Engine::Get_InfoSubject()->DestroyInstance();
}


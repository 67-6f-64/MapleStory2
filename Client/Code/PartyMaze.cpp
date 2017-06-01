#include "stdafx.h"
#include "PartyMaze.h"

#include "Include.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Layer.h"
#include "GameObject.h"
#include "KeyMgr.h"
#include "MazeFloor.h"
#include "Player.h"
#include "FreeCamera.h"
#include "StaticCamera.h"
#include "DynamicCamera.h"
#include "PlayerLookCamera.h"
#include "Cube.h"
#include "MazeWall.h"
#include "chat.h"
#include "SkyBox.h"
#include "MazePlayer.h"
#include "MazePosObserver.h"
#include "transform.h"

#include "PlayerWin.h"
#include "EndFlag.h"
#include "StartFlag.h"

#include "CubePlayerLeftFoot.h"
#include "CubePlayerRightFoot.h"

#include "ClientNetwork.h"

CPartyMaze::CPartyMaze(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CScene(pDevice)
	, m_pNet(NETMGR)
	, m_pMazePosObserver(nullptr)
	, m_fTime(0.f)
{
	m_wSceneType = 4;
}

CPartyMaze::~CPartyMaze(void)
{
	Release();
}

HRESULT CPartyMaze::Initialize(void)
{
	HRESULT		hr = NULL;

	Engine::Get_SoundMgr()->StopSoundAll();
	Engine::Get_SoundMgr()->PlayBGM(L"Zakum.mp3", Engine::CHANNEL_PARTYMAZE);
	Engine::Get_SoundMgr()->PlayBGM(L"elaborate_thunder-Mike_Koenig-1877244752.wav", Engine::CHANNEL_PARTYMAZE_THUNDER);

	/////////////////////////////////////////////////////////
	// 네트워크에 레이어 세팅
	m_pNet->SetLayer(&m_mapLayer);
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);

	m_pbSceneFlagArr = new bool[FLAG_END];
	memset(m_pbSceneFlagArr, false, FLAG_END * sizeof(bool));
	m_pNet->SetSceneFlagArr(&m_pbSceneFlagArr);

	//cout << "- 미로 플래그 배열 주소: " << m_pbSceneFlagArr << endl;
	///////////////////////////////////////////////////////////

	// 오브젝트 생성하기 전에 옵저버 구독 설정
	m_pMazePosObserver = CMazePosObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pMazePosObserver);

	hr = Add_Environment_Layer();	FAILED_CHECK(hr);
	hr = Add_GameLogic_Layer();		FAILED_CHECK(hr);
	hr = Add_UI_Layer();			FAILED_CHECK(hr);

	Engine::NETDATA tData;
	tData.chKey = 72;

	// 플레이어 생성 요청
	tData.eMessage = Engine::NETMSG_CLIENT_CREATE_PLAYER;
	m_pNet->SendMsg(tData);

	++m_iPlayerCreateCnt;


	// Directional Light Test
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 0.2f;
	light.Diffuse.g = 0.5f;
	light.Diffuse.b = 0.2f;

	// D3DLIGHT_DIRECTION 일 때

	D3DXVECTOR3 vecDir = { 1.f, -1.f, 1.f };
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	m_pDevice->SetLight(500, &light);
	m_pDevice->LightEnable(500, TRUE);
	
	return S_OK;
}

void CPartyMaze::Update(void)
{
	// 플래그에 따라 함수 실행
	FlagFuntion();

	Engine::CScene::Update();

	int iPlayerWin = m_pMazePosObserver->GetPlayerWin();

	// 미로가 끝나면 넘어가자
	if (iPlayerWin != -1 && m_fTime < -10.f) // 처음 10초동안은 못넘어간다
	{
		// 승리 UI
		Round_Player_Win(iPlayerWin);
		m_fTime = 5.f;
	}
		
	// 5초 뒤 Scene 전환
	if( CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADENTER) || (iPlayerWin != -1 && m_fTime < 0.f))
	{
		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		tData.wSubMessage = (WORD)SCENE_PARTYROOM;

		m_pNet->SendMsg(tData);
	}

	m_fTime -= Engine::Get_TimeMgr()->GetTime();
}

void CPartyMaze::Render(void)
{
	Engine::CScene::Render();

	// 시간 Font 출력은 MazeFloor에 있다.

	if (m_bSceneChange && m_fTime < 0.f)
	{
		//cout << "CPartyMaze - 씬 변경" << endl;
		m_pNet->SetSceneFlagArrNull();
		m_pNet->SetLayer(nullptr);
		Engine::Get_Management()->SceneChange(CSceneSelector((SCENEID)m_wSceneType), m_pDevice);
		return;
	}
}

CPartyMaze* CPartyMaze::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyMaze*	pScene = new CPartyMaze(pDevice);
	if (FAILED(pScene->Initialize()))
		Safe_Delete(pScene);

	return pScene;
}

HRESULT CPartyMaze::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_ENVIROMENT))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_ENVIROMENT];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CSkyBox::Create(m_pDevice, PARTYMAZE_SKYBOX);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ASkyBox", pGameObject);

	pGameObject = CMazeFloor::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Floor", pGameObject);

	// 미로 벽
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../bin/MapData/MazeWallData.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		WALLDATA* pWallData = new WALLDATA;

		ReadFile(hFile, pWallData, sizeof(WALLDATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(pWallData);
			break;
		}

		pGameObject = CMazeWall::Create(m_pDevice, (WORD)pWallData->eWallDir);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->SetPos(pWallData->vPos);

		pLayer->AddObject(L"MazeWall", pGameObject);

		Engine::Get_InfoSubject()->AddData(MESSAGE_PARTYMAZEWALL_CREATE, pGameObject);

		Engine::Safe_Delete(pWallData);
	}
	CloseHandle(hFile);
	
	// 조명
	hFile = CreateFile(L"../bin/MapData/MazeLightData.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		LIGHTDATA* pLightData = new LIGHTDATA;

		ReadFile(hFile, pLightData, sizeof(LIGHTDATA), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(pLightData);
			break;
		}

		Add_Light(pLightData->vPos + D3DXVECTOR3(0.f, 2.f, 0.f), pLightData->iLightCount);

		Engine::Get_InfoSubject()->AddData(MESSAGE_PARTYMAZELIGHT_CREATE, &make_pair(pLightData->vPos, pLightData->iLightCount));

		Engine::Safe_Delete(pLightData);
	}
	CloseHandle(hFile);

	// Flag
	pGameObject = CStartFlag::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(1.8f, 2.6f, -2.0f));
	pLayer->AddObject(L"StartFlag", pGameObject);

	pGameObject = CEndFlag::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(102.004f, 2.6f, -113.587f));
	pLayer->AddObject(L"EndFlag", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIROMENT, pLayer));

	return S_OK;
}

HRESULT CPartyMaze::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_GAMELOGIC))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_GAMELOGIC];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	// 플레이어
	pGameObject = CMazePlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObjectFront(L"Player", pGameObject);

	// 클라이언트 세팅
	dynamic_cast<CMazePlayer*>(pGameObject)->SetClient();
	// 인덱스 세팅
	pGameObject->SetServerIndex(g_iClientIndex);		// 서버 접속할 때 받아놓은 인덱스

	//cout << endl << " - " << g_iClientIndex + 1 << " 번 클라이언트" << endl << endl;

	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent(L"Transform");

	Engine::CGameObject*		pCubeFoot = NULL;

	pCubeFoot = CCubePlayerLeftFoot::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pCubeFoot, E_FAIL);
	dynamic_cast<CCubePlayerLeftFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CMazePlayer*>(pGameObject)->GetPlayerState()));
	pLayer->AddObject(L"PlayerLeftFoot", pCubeFoot);

	pCubeFoot = CCubePlayerRightFoot::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pCubeFoot, E_FAIL);
	dynamic_cast<CCubePlayerRightFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CMazePlayer*>(pGameObject)->GetPlayerState()));
	pLayer->AddObject(L"PlayerRightFoot", pCubeFoot);


	// 충돌 옵저버
	Engine::Get_InfoSubject()->AddDataFront(MESSAGE_PLAYER_POSITION, &pGameObject->GetTransform()->m_vPos);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CPartyMaze::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_UI))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	//// Free Camera
	/*pGameObject = CFreeCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"FreeCamera", pGameObject);*/

	// Dynamic Camera
	Engine::CTransform* pTransform = (Engine::CTransform*)m_mapLayer[LAYER_GAMELOGIC]->GetComponent(L"Player", L"Transform");
	pGameObject = CDynamicCamera::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"DynamicCamera", pGameObject);

	// 채팅
	pGameObject = CChat::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Chat", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CPartyMaze::Release(void)
{
	Engine::Get_InfoSubject()->UnSubscribe(m_pMazePosObserver);

	Engine::Safe_Delete(m_pMazePosObserver);
}

void CPartyMaze::Add_Light(D3DXVECTOR3 vecPos, int iLightCount)
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
	light.Range = 5.f;

	m_pDevice->SetLight(iLightCount, &light);
	m_pDevice->LightEnable(iLightCount, FALSE);
}

HRESULT CPartyMaze::Round_Player_Win(int iPlayerWin)
{
	// PlayerWin UI
	Engine::CGameObject* pGameObject = CPlayerWin::Create(m_pDevice, iPlayerWin);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_mapLayer[LAYER_UI]->AddObject(L"PlayerWin", pGameObject);

	return S_OK;
}

void CPartyMaze::FlagFuntion(void)
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

void CPartyMaze::PlayerCreate(void)
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
			Engine::CGameObject*	pObj = CMazePlayer::Create(m_pDevice);
			pObj->SetServerIndex(i);
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(_T("Player"), pObj);

			//cout << i + 1 << "번 플레이어 생성" << endl;

			Engine::CTransform* pTransform = (Engine::CTransform*)pObj->GetComponent(L"Transform");

			Engine::CGameObject*		pCubeFoot = NULL;

			pCubeFoot = CCubePlayerLeftFoot::Create(m_pDevice, pTransform);
			dynamic_cast<CCubePlayerLeftFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CMazePlayer*>(pObj)->GetPlayerState()));
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(L"PlayerLeftFoot", pCubeFoot);

			pCubeFoot = CCubePlayerRightFoot::Create(m_pDevice, pTransform);
			dynamic_cast<CCubePlayerRightFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CMazePlayer*>(pObj)->GetPlayerState()));
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(L"PlayerRightFoot", pCubeFoot);
		}
	}

	++m_iPlayerCreateCnt;

}

void CPartyMaze::PlayerUpdate(void)
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

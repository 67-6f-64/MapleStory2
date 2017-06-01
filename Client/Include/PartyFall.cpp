#include "stdafx.h"
#include "PartyFall.h"

#include "ClientNetwork.h"
#include "Layer.h"
#include "GameObject.h"
#include "StaticCamera.h"
#include "PartyFallPlayer.h"
#include "Frustum.h"
#include "Export_Function.h"

#include "Tile.h"
#include "PartyFallCube.h"
#include "PartyFallBackground.h"
#include "PartyFallNumber.h"
#include "PlayerWin.h"
#include "Chat.h"
#include "PartyFallItem.h"
#include "SceneSelector.h"
#include "CubeObserver.h"
#include "ItemObserver.h"
#include "RoundObserver.h"
#include "KeyMgr.h"

int CPartyFall::m_iRound = 0;

CPartyFall::CPartyFall(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pNet(NETMGR)
, m_fTime(3.f)
, m_iPlayerCount(0)
, m_pCubeObserver(nullptr)
, m_pItemObserver(nullptr)
, m_pRoundObserver(nullptr)
, m_bRoundFinish(false)
{
	m_wSceneType = 3;
}


CPartyFall::~CPartyFall()
{
	Release();
}

HRESULT CPartyFall::Initialize(void)
{
	HRESULT		hr = NULL;

	srand(unsigned(time(NULL)));

	// 네트워크에 레이어 세팅
	m_pNet->SetLayer(&m_mapLayer);
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);

	// 플레이어 Initialize 전에 구독
	m_pCubeObserver = CCubeObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pCubeObserver);

	m_pItemObserver = CItemObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pItemObserver);

	m_pRoundObserver = CRoundObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pRoundObserver);

	hr = Add_Environment_Layer();	FAILED_CHECK(hr);
	hr = Add_GameLogic_Layer();		FAILED_CHECK(hr);
	hr = Add_UI_Layer();			FAILED_CHECK(hr);
	Round_Start_UI();
	/////////////////////////////////////////////////////////
	Engine::NETDATA tData;
	tData.chKey = 72;
	
	// 플레이어 생성 요청
	tData.eMessage = Engine::NETMSG_CLIENT_CREATE_PLAYER;
	m_pNet->SendMsg(tData);

	return S_OK;
}

void CPartyFall::Update(void)
{
	// Frustum을 만든다.
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	CFrustumMgr::GetInstance()->MakeFrustumPlane(matView, matProj);

	if (m_pRoundObserver->IsOnlyOneSurvive() != -1 && m_bRoundFinish == false)
	{
		Round_Player_Win(m_pRoundObserver->IsOnlyOneSurvive()); // UI 출력
		m_bRoundFinish = true; // 라운드 종료
		m_iRound++;
	}
		
	if (m_bRoundFinish == TRUE) // 3초간 Player 1 Win! 을 보여준다.
		m_fTime -= Engine::Get_TimeMgr()->GetTime();

	if (m_fTime < 0.f)
	{
		m_fTime = 10.f;

		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		if (m_iRound == 3)
		{
			m_iRound = 0;
			tData.wSubMessage = (WORD)SCENE_PARTYROOM;
		}
		else
			tData.wSubMessage = (WORD)SCENE_PARTYFALL;

		cout << m_iRound << endl;

		m_pNet->SendMsg(tData);
	}

	Engine::CScene::Update();
}

void CPartyFall::Render(void)
{
	Engine::CScene::Render();

	if (m_bSceneChange) // Render에 있는 이유 : Stage의 Update부터 시작하기 위해
	{
		m_pNet->SetLayer(nullptr);
		Engine::Get_Management()->SceneChange(CSceneSelector((SCENEID)m_wSceneType), m_pDevice);
		return;
	}
}

CPartyFall * CPartyFall::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPartyFall*	pScene = new CPartyFall(pDevice);
	if (FAILED(pScene->Initialize()))
		Safe_Delete(pScene);

	return pScene;
}

HRESULT CPartyFall::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_ENVIROMENT))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_ENVIROMENT];

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	// 배경
	pGameObject = CPartyFallBackground::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"PartyFallBackground", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIROMENT, pLayer));

	return S_OK;
}

HRESULT CPartyFall::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_GAMELOGIC))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_GAMELOGIC];
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	// 큐브 (플레이어에서 옵저버를 생성해야 AddData가 가능하다)
	for (int i = 0; i < PARTYFALL_CUBECNT; ++i)
	{
		for (int j = 0; j < PARTYFALL_CUBECNT; ++j)
		{
			pGameObject = CPartyFallCube::Create(m_pDevice);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->SetPos(D3DXVECTOR3(j * 2.f, -2.f, i * 2.f));
			dynamic_cast<CPartyFallCube*>(pGameObject)->SetInitialPos(D3DXVECTOR3(j * 2.f, -2.f, i * 2.f));
			pLayer->AddObject(L"PartyFallCube", pGameObject);

			Engine::Get_InfoSubject()->AddData(MESSAGE_PARTYFALLCUBE_CREATE, pGameObject);
		}
	}

	// 플레이어
	pGameObject = CPartyFallPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObjectFront(L"Player", pGameObject);

	SetPlayerStartPos(pGameObject);

	// 클라이언트 세팅
	dynamic_cast<CPartyFallPlayer*>(pGameObject)->SetClient();
	// 인덱스 세팅
	pGameObject->SetServerIndex(g_iClientIndex);		// 서버 접속할 때 받아놓은 인덱스

	// 아이템
	for (int i = 1; i <= 2; ++i)
	{
		pGameObject = CPartyFallItem::Create(m_pDevice);
		pGameObject->SetPos(D3DXVECTOR3(20.f * i, 20.f, 20.f * i));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->AddObject(L"PartyFallItem", pGameObject);

		Engine::Get_InfoSubject()->AddData(MESSAGE_PARTYFALLITEM_CREATE, pGameObject);
	}

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CPartyFall::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_UI))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	// Static Camera
	Engine::CTransform* pTransform = (Engine::CTransform*)m_mapLayer[LAYER_GAMELOGIC]->GetComponent(L"Player", L"Transform");

	pGameObject = CStaticCamera::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	// 채팅
	pGameObject = CChat::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Chat", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CPartyFall::Release(void)
{
	Engine::Get_InfoSubject()->UnSubscribe(m_pItemObserver);
	Engine::Get_InfoSubject()->UnSubscribe(m_pCubeObserver);
	Engine::Get_InfoSubject()->UnSubscribe(m_pRoundObserver);

	Engine::Safe_Delete(m_pCubeObserver);
	Engine::Safe_Delete(m_pItemObserver);
	Engine::Safe_Delete(m_pRoundObserver);
}

void CPartyFall::SetPlayerStartPos(Engine::CGameObject* pObj)
{
	switch (g_iClientIndex)
	{
	case 0:
		pObj->SetPos(D3DXVECTOR3(0.f, 0.f, 0.f));
		break;
	case 1:
		pObj->SetPos(D3DXVECTOR3(((PARTYFALL_CUBECNT - 1)*2.f), 0.f, 0.f));
		break;
	case 2:
		pObj->SetPos(D3DXVECTOR3(0.f, 0.f, ((PARTYFALL_CUBECNT - 1)*2.f)));
		break;
	case 3:
		pObj->SetPos(D3DXVECTOR3(((PARTYFALL_CUBECNT - 1)*2.f), 0.f, ((PARTYFALL_CUBECNT - 1)*2.f)));
		break;
	}
}

HRESULT CPartyFall::Round_Start_UI(void)
{
	// Number UI
	Engine::CGameObject* pGameObject = CPartyFallNumber::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_mapLayer[LAYER_UI]->AddObject(L"PartyFallNumber", pGameObject);

	return S_OK;
}

HRESULT CPartyFall::Round_Player_Win(int iPlayerWin)
{
	// PlayerWin UI
	Engine::CGameObject* pGameObject = CPlayerWin::Create(m_pDevice, iPlayerWin);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_mapLayer[LAYER_UI]->AddObject(L"PlayerWin", pGameObject);

	return S_OK;
}

/
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

	Engine::Get_SoundMgr()->StopSoundAll();

	switch (m_iRound)
	{
	case 0:
		Engine::Get_SoundMgr()->PlayBGM(L"[MapleStory 2 BGM] Boss 03 (2).mp3", Engine::CHANNEL_PARTYFALL);
		break;

	case 1: 
		Engine::Get_SoundMgr()->PlayBGM(L"BGM_Madria_Theme_01.mp3", Engine::CHANNEL_PARTYFALL);
		break;

	case 2:
		Engine::Get_SoundMgr()->PlayBGM(L"Zakum.mp3", Engine::CHANNEL_PARTYFALL);
		break;

	default:
		break;
	}

	srand(unsigned(time(NULL)));

	// ��Ʈ��ũ�� ���̾� ����
	m_pNet->SetLayer(&m_mapLayer);
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);

	m_pbSceneFlagArr = new bool[FLAG_END];
	memset(m_pbSceneFlagArr, false, FLAG_END * sizeof(bool));
	m_pNet->SetSceneFlagArr(&m_pbSceneFlagArr);

	//cout << "- ������Ƴ��� �÷��� �迭 �ּ�: " << m_pbSceneFlagArr << endl;
	///////////////////////////////////////////////////////////

	// �÷��̾� Initialize ���� ����
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
	
	// �÷��̾� ���� ��û
	tData.eMessage = Engine::NETMSG_CLIENT_CREATE_PLAYER;
	m_pNet->SendMsg(tData);
	
	++m_iPlayerCreateCnt;

	return S_OK;
}

void CPartyFall::Update(void)
{
	// �÷��׿� ���� �Լ� ����
	FlagFuntion();

	// Frustum�� �����.
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	CFrustumMgr::GetInstance()->MakeFrustumPlane(matView, matProj);

	if (m_pRoundObserver->IsOnlyOneSurvive() != -1 && m_bRoundFinish == false)
	{
		Round_Player_Win(m_pRoundObserver->IsOnlyOneSurvive()); // UI ���
		m_bRoundFinish = true; // ���� ����
		m_iRound++;
	}
		
	if (m_bRoundFinish == TRUE) // 3�ʰ� Player 1 Win! �� �����ش�.
		m_fTime -= Engine::Get_TimeMgr()->GetTime();

	if (m_fTime < 0.f)
	{
		m_fTime = 10.f;

		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		// ������ ����� ������ �� �÷��̾�(g_CLientNumber�� 0��?) �ָ� ����.
		if (g_iClientIndex == 0)
		{
			if (m_iRound == 3)
			{
				m_iRound = 0;
				tData.wSubMessage = (WORD)SCENE_PARTYROOM;
			}
			else
				tData.wSubMessage = (WORD)SCENE_PARTYFALL;

			m_pNet->SendMsg(tData);			
		}		
	}

	Engine::CScene::Update();

	//cout << "�÷��̾� ��: " << m_iPlayerCreateCnt << endl;
}

void CPartyFall::Render(void)
{
	Engine::CScene::Render();

	if (m_bSceneChange) // Render�� �ִ� ���� : Stage�� Update���� �����ϱ� ����
	{

		m_pNet->SetSceneFlagArrNull();
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

	// ���
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

	// ť�� (�÷��̾�� �������� �����ؾ� AddData�� �����ϴ�)
	for (int i = 0; i < PARTYFALL_CUBECNT; ++i)
	{
		for (int j = 0; j < PARTYFALL_CUBECNT; ++j)
		{
			pGameObject = CPartyFallCube::Create(m_pDevice, m_iRound + 1);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pGameObject->SetPos(D3DXVECTOR3(j * 2.f, -2.f, i * 2.f));
			dynamic_cast<CPartyFallCube*>(pGameObject)->SetInitialPos(D3DXVECTOR3(j * 2.f, -2.f, i * 2.f));
			pLayer->AddObject(L"PartyFallCube", pGameObject);

			Engine::Get_InfoSubject()->AddData(MESSAGE_PARTYFALLCUBE_CREATE, pGameObject);
		}
	}

	// �÷��̾�
	pGameObject = CPartyFallPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObjectFront(L"Player", pGameObject);

	SetPlayerStartPos(pGameObject);

	// Ŭ���̾�Ʈ ����
	dynamic_cast<CPartyFallPlayer*>(pGameObject)->SetClient();
	// �ε��� ����
	pGameObject->SetServerIndex(g_iClientIndex);		// ���� ������ �� �޾Ƴ��� �ε���

	//cout << endl << " - " << g_iClientIndex + 1 << " �� Ŭ���̾�Ʈ" << endl << endl;

	// ������
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

	// ä��
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

void CPartyFall::FlagFuntion(void)
{
	if (m_pbSceneFlagArr[FLAG_PLAYER_CREATE])
	{
		PlayerCreate();		// �ѹ� ���� �� �־����� �÷��׸� �ǵ�����.
		m_pbSceneFlagArr[FLAG_PLAYER_CREATE] = false;
	}

	if (m_pbSceneFlagArr[FLAG_PLAYER_UPDATE])
	{
		PlayerUpdate();
		m_pbSceneFlagArr[FLAG_PLAYER_UPDATE] = false;
	}
}

void CPartyFall::PlayerCreate(void)
{
	// ���� ����Ǿ� �ִ� Ŭ���̾�Ʈ�� ������ �����´�.
	const size_t iCnt = ((Engine::NETDATA*)(m_pNet->GetMapNetData(WORD(FLAG_PLAYER_CREATE))))->iIndex;


	auto iter = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->begin();
	auto iter_end = m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->end();

	if (iCnt <= m_mapLayer[LAYER_GAMELOGIC]->GetObjList(_T("Player"))->size())
		return;

	// Ŭ���̾�Ʈ ����ŭ ���� ���� �Ǻ�
	vector<bool> vPlayer;
	for (size_t i = 0; i < iCnt; ++i)
		vPlayer.push_back(false);

	// ���� ������� �ִ� �÷��̾�� true�� ������Ѽ� �ȸ�������ִ� �� ���� ����
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
		// �÷��̾ �ȸ������ �ε����� ���� : false�� �ε���
		if (!vPlayer[i])
		{
			Engine::CGameObject*	pObj = CPartyFallPlayer::Create(m_pDevice);
			pObj->SetServerIndex(i);
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(_T("Player"), pObj);

			//cout << i + 1 << "�� �÷��̾� ����" << endl;
		}
	}

	++m_iPlayerCreateCnt;

}

void CPartyFall::PlayerUpdate(void)
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

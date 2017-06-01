#include "stdafx.h"
#include "Stage.h"
#include "Include.h"
#include "Layer.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "Component.h"
#include "Transform.h"
#include "Frustum.h"
#include "KeyMgr.h"
#include "SceneSelector.h"

#include "Player.h"
#include "StaticCamera.h"
#include "Wall.h"
#include "Floor.h"
#include "Ceiling.h"
#include "Cube.h"
#include "MainUI.h"
#include "chat.h"
#include "StageBackground.h"
#include "SkyBox.h"

#include "FreeCamera.h"

#include "ClientNetwork.h"
#include "PortalObserver.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CScene(pDevice)
	, m_pNet(NETMGR) 
{
	m_wSceneType = 1;
}

CStage::~CStage(void)
{
	Release();
}

HRESULT CStage::Initialize(void)
{
	HRESULT      hr = NULL;

	Engine::Get_SoundMgr()->StopSoundAll();
	Engine::Get_SoundMgr()->PlayBGM(L"[SHANA]�ȳ�ٴ� - ������ ������(��Ҹ�).mp3", Engine::CHANNEL_STAGE);

	/////////////////////////////////////////////////////////
	// ��Ʈ��ũ�� ���̾� ����
	m_pNet->SetLayer(&m_mapLayer);
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);

	m_pbSceneFlagArr = new bool[FLAG_END];
	memset(m_pbSceneFlagArr, false, FLAG_END * sizeof(bool));
	m_pNet->SetSceneFlagArr(&m_pbSceneFlagArr);

	//cout << "- �������� �÷��� �迭 �ּ�: " << m_pbSceneFlagArr << endl;
	/////////////////////////////////////////////////////////

	hr = Add_Environment_Layer();   FAILED_CHECK(hr);
	hr = Add_GameLogic_Layer();      FAILED_CHECK(hr);
	hr = Add_UI_Layer();         FAILED_CHECK(hr);

	// �÷��̾� ���� ��û
	Engine::NETDATA tData;
	tData.chKey = 72;
	
	tData.eMessage = Engine::NETMSG_CLIENT_CREATE_PLAYER;
	m_pNet->SendMsg(tData);

	++m_iPlayerCreateCnt;

	return S_OK;
}

void CStage::Update(void)
{
	// Frustum�� �����.
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	CFrustumMgr::GetInstance()->MakeFrustumPlane(matView, matProj);

	// �÷��׿� ���� �Լ� ����
	FlagFuntion();

	Engine::CScene::Update();

	// TSW �� ���� ����
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADENTER) ||
		CKeyMgr::GetInstance()->OnceKeyDown(DIK_END))
	{
		// ���⼭ �� ���� �޼��� ������ ����
		Engine::NETDATA   tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		// �ʿ信 ���� �ٸ������� ���� ����
		tData.wSubMessage = (WORD)SCENE_PARTYROOM;

		m_pNet->SendMsg(tData);		
	}

	//cout << "�÷��̾� ��: " << m_iPlayerCreateCnt << endl;
}

void CStage::Render(void)
{
	//if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_RETURN)) // Render�� �ִ� ���� : Stage�� Update���� �����ϱ� ����
	//{
	//   Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_PARTYROOM), m_pDevice);
	//   return;
	//}



	if (m_bSceneChange) // Render�� �ִ� ���� : Stage�� Update���� �����ϱ� ����
	{
		m_pNet->SetSceneFlagArrNull();
		m_pNet->SetLayer(nullptr);
		Engine::Get_Management()->SceneChange(CSceneSelector((SCENEID)m_wSceneType), m_pDevice);
		return;
	}

	Engine::CScene::Render();
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage*   pScene = new CStage(pDevice);
	if (FAILED(pScene->Initialize()))
		Safe_Delete(pScene);

	return pScene;
}


HRESULT CStage::Add_Environment_Layer(void)
{
	Engine::CLayer*      pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_ENVIROMENT))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_ENVIROMENT];

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = NULL;

	pGameObject = CSkyBox::Create(m_pDevice, STAGE_SKYBOX);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ASkyBox", pGameObject);

	// ť��
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../bin/MapData/StageCubeMap.Dat",
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

		pGameObject = CCube::Create(m_pDevice);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);

		dynamic_cast<CCube*>(pGameObject)->SetCubeTexture(pCubeData->dwTextureNum);
		dynamic_cast<CCube*>(pGameObject)->SetCubeSequence(pCubeData->eCubeSequence);
		dynamic_cast<CCube*>(pGameObject)->SetCubeSide(pCubeData->bOutSide);

		dynamic_cast<CCube*>(pGameObject)->SetCubeOrignalPos(pCubeData->vPos);
		dynamic_cast<CCube*>(pGameObject)->SetCubeIndex(i++);

		pGameObject->SetPos(pCubeData->vPos);

		pLayer->AddObject(L"Cube", pGameObject);

		Engine::Safe_Delete(pCubeData);
	}
	CloseHandle(hFile);

	// ���
	/*pGameObject = CStageBackground::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StageBackground", pGameObject);*/

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIROMENT, pLayer));

	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	Engine::CLayer*      pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_GAMELOGIC))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_GAMELOGIC];

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = nullptr;

	// �÷��̾�
	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(9999.f, 0.f, 0.f));
	pLayer->AddObjectFront(L"Player", pGameObject);

	// Ŭ���̾�Ʈ ����
	dynamic_cast<CPlayer*>(pGameObject)->SetClient();
	// �ε��� ����
	pGameObject->SetServerIndex(g_iClientIndex);      // ���� ������ �� �޾Ƴ��� �ε���

	//cout << endl << " - " << g_iClientIndex + 1 << " �� Ŭ���̾�Ʈ" << endl << endl;

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CStage::Add_UI_Layer(void)
{
	Engine::CLayer*      pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_UI))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_UI];

	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	// Static Camera
	/*Engine::CTransform* pTransform = (Engine::CTransform*)m_mapLayer[LAYER_GAMELOGIC]->GetComponent(L"Player", L"Transform");

	pGameObject = CStaticCamera::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);*/

	pGameObject = CFreeCamera::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"FreeCamera", pGameObject);

	//// HpMpGauge
	//pGameObject = CMainUI::Create(m_pDevice);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->AddObject(L"HpMpGague", pGameObject);

	// ä��
	pGameObject = CChat::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Chat", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CStage::Release(void)
{
}

void CStage::FlagFuntion(void)
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

void CStage::PlayerCreate(void)
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

	/*cout << endl << "-------------------------------" << endl;
	for (size_t i = 0; i < vPlayer.size(); ++i)
	{
		cout << "[ " << i + 1 << " | " << vPlayer[i] << "] ";
	}
	cout << endl;*/

	for (size_t i = 0; i < vPlayer.size(); ++i)
	{
		// �÷��̾ �ȸ������ �ε����� ���� : false�� �ε���
		if (!vPlayer[i])
		{
			Engine::CGameObject*	pObj = CPlayer::Create(m_pDevice);
			pObj->SetServerIndex(i);
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(_T("Player"), pObj);

			//cout << i + 1 << "�� �÷��̾� ����" << endl;
		}		
	}
	
	++m_iPlayerCreateCnt;	
}

void CStage::PlayerUpdate(void)
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

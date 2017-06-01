#include "stdafx.h"
#include "BattleGround.h"
#include "ClientNetwork.h"
#include "Export_Function.h"
#include "Chat.h"
#include "SceneSelector.h"
#include "Layer.h"
#include "GameObject.h"
#include "BattleFloor.h"

#include "SkyBox.h"
#include "BattleGroundPlayer.h"
#include "PlayerLookCamera.h"
#include "BulletObserver.h"
#include "Battle_ItemObserver.h"
#include "MapObserver.h"
#include "CubePlayerLeftFoot.h"
#include "CubePlayerRightFoot.h"
#include "keymgr.h"

#include "Item.h"
#include "Meteor.h"
#include "House.h"
#include "LowHp.h"

#include "CubePlayerLeftFoot.h"
#include "CubePlayerRightFoot.h"

CBattleGround::CBattleGround(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CScene(pDevice)
	, m_pNet(NETMGR)
	, m_pBulletObserver(nullptr)
	, m_pItemObserver(nullptr)
	, m_pMapObserver(nullptr)
	, m_fSpawnTime(4.f)
	, m_fMeteorTime(0.1f)
{
}


CBattleGround::~CBattleGround()
{
	Release();
}

HRESULT CBattleGround::Initialize(void)
{
	//cout << "CBattleGround - �̴ϼȶ����� ����" << endl;
	HRESULT		hr = NULL;
	
	Engine::Get_SoundMgr()->StopSoundAll();
	Engine::Get_SoundMgr()->PlayBGM(L"[MapleStory 2 BGM] Boss 03 (1).mp3", Engine::CHANNEL_BATTLEGROUND);

	srand(unsigned(time(NULL)));

	m_pBulletObserver = CBulletObserver::Create(); // Bullet Observer
	Engine::Get_InfoSubject()->Subscribe(m_pBulletObserver);

	m_pItemObserver = CBattle_ItemObserver::Create(); // Item Observer
	Engine::Get_InfoSubject()->Subscribe(m_pItemObserver);

	m_pMapObserver = CMapObserver::Create(); // Map Observer
	Engine::Get_InfoSubject()->Subscribe(m_pMapObserver);

	// ��Ʈ��ũ�� ���̾� ����
	m_pNet->SetLayer(&m_mapLayer);
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);

	m_pbSceneFlagArr = new bool[FLAG_END];
	memset(m_pbSceneFlagArr, false, FLAG_END * sizeof(bool));
	m_pNet->SetSceneFlagArr(&m_pbSceneFlagArr);

	//cout << "- ��Ʋ �׶��� �÷��� �迭 �ּ�: " << m_pbSceneFlagArr << endl;
	/////////////////////////////////////////////////////////////

	hr = Add_Environment_Layer();	FAILED_CHECK(hr);
	hr = Add_GameLogic_Layer();		FAILED_CHECK(hr);
	hr = Add_UI_Layer();			FAILED_CHECK(hr);

	/////////////////////////////////////////////////////////
	Engine::NETDATA tData;
	tData.chKey = 72;

	// �÷��̾� ���� ��û
	tData.eMessage = Engine::NETMSG_CLIENT_CREATE_PLAYER;
	m_pNet->SendMsg(tData);

	++m_iPlayerCreateCnt;

	m_iBulletCreatePlayerIndex = g_iClientIndex;
	Engine::Get_InfoSubject()->AddData(MESSAGE_BULLET_PLAYER_INDEX, &m_iBulletCreatePlayerIndex);

	m_fItemHandle = CreateFile(L"../bin/ItemData/ItemData.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	m_fMeteorHandle = CreateFile(L"../bin/MeteorData/MeteorData.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//D3DXVECTOR3 vecItem = { 0.f, 0.f, 0.f };
	//HANDLE hFile = CreateFile(L"../bin/MeteorData/MeteorData.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//DWORD dwByte = 0;
	//// �� ����Ÿ ����
	//for (int i = 0; i < 2400; ++i)
	//{
	//	vecItem = D3DXVECTOR3(float(rand() % 200), 10.f, float(rand() % 200));

	//	WriteFile(hFile, &vecItem, sizeof(D3DXVECTOR3), &dwByte, NULL);
	//}
	//CloseHandle(hFile);
	
	return S_OK;
}

void CBattleGround::Update(void)
{	
	// �÷��׿� ���� �Լ� ����
	FlagFuntion();

	SpawnItem();
	SpawnMeteor();

	// �Ѿ˿� ������ UI ������ش�.
	if (m_pBulletObserver->GetShowPlayerHit() == true)
	{
		CLowHp* pPlayerHit = CLowHp::Create(m_pDevice);
		m_mapLayer[LAYER_UI]->AddObject(L"PlayerHit", pPlayerHit);

		m_pBulletObserver->SetShowPlayerHit(false);
	}
		
	Engine::CScene::Update();

	m_pBulletObserver->Update();

	if(CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADENTER))
	{
		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHANGE;

		tData.wSubMessage = (WORD)SCENE_PARTYROOM;

		m_pNet->SendMsg(tData);
	}

	//cout << "�÷��̾� ��: " << m_iPlayerCreateCnt << endl;
}

void CBattleGround::Render(void)
{
	Engine::CScene::Render();

	m_pBulletObserver->Render();

	if (m_bSceneChange) // Render�� �ִ� ���� : Stage�� Update���� �����ϱ� ����
	{
		//cout << "BattleGround - �� ����" << endl;
		m_pNet->SetSceneFlagArrNull();
		m_pNet->SetLayer(nullptr);
		Engine::Get_Management()->SceneChange(CSceneSelector((SCENEID)m_wSceneType), m_pDevice);
		return;
	}
}

CBattleGround * CBattleGround::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBattleGround*	pScene = new CBattleGround(pDevice);
	if (FAILED(pScene->Initialize()))
		Safe_Delete(pScene);

	return pScene;
}

void CBattleGround::SpawnItem(void)
{
	m_fSpawnTime -= Engine::Get_TimeMgr()->GetTime();

	DWORD dwByte = 0;

	D3DXVECTOR3 vecItem;

	if (m_fSpawnTime < 0.f)
	{
		// n�ʸ��� 1���� �������� �����ϰ� �Ѹ���. 
		m_fSpawnTime = 3.f;

		ReadFile(m_fItemHandle, &vecItem, sizeof(D3DXVECTOR3), &dwByte, NULL);

		if (dwByte == 0)
		{
			CloseHandle(m_fItemHandle);
			m_fItemHandle = CreateFile(L"../bin/ItemData/ItemData.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			return;
		}

		CItem* pItem = CItem::Create(m_pDevice);
		pItem->SetPos(vecItem);
		pItem->SetItemType(ITEM_PORTION);

		m_mapLayer[LAYER_GAMELOGIC]->AddObject(L"BattleGround_Item", pItem);

		Engine::Get_InfoSubject()->AddData(MESSAGE_ITEM_CREATE, pItem);
	}
}

void CBattleGround::SpawnMeteor(void)
{
	m_fMeteorTime -= Engine::Get_TimeMgr()->GetTime();

	DWORD dwByte = 0;

	D3DXVECTOR3 vecMeteor;

	if (m_fMeteorTime < 0.f)
	{
		m_fMeteorTime = 0.1f;

		ReadFile(m_fMeteorHandle, &vecMeteor, sizeof(D3DXVECTOR3), &dwByte, NULL);

		if (dwByte == 0)
		{
			CloseHandle(m_fMeteorHandle);
			m_fMeteorHandle = CreateFile(L"../bin/MeteorData/MeteorData.Dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			return;
		}

		CMeteor* pMeteor = CMeteor::Create(m_pDevice);
		vecMeteor.y = 20.f;
		pMeteor->SetPos(vecMeteor);

		m_mapLayer[LAYER_GAMELOGIC]->AddObject(L"BattleGround_Meteor", pMeteor);

		Engine::Get_InfoSubject()->AddData(MESSAGE_METEOR_CREATE, pMeteor);
	}
}

HRESULT CBattleGround::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_ENVIROMENT))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_ENVIROMENT];

	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CBattleFloor::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"BattleFloor", pGameObject);

	pGameObject = CSkyBox::Create(m_pDevice, BATTLEGROUND_SKYBOX);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ASkyBox", pGameObject);

	pGameObject = CHouse::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(50.f, 10.1f, 150.f));
	dynamic_cast<CHouse*>(pGameObject)->SetHouseColor(0);
	pLayer->AddObject(L"Battle_House", pGameObject);

	pGameObject = CHouse::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->SetPos(D3DXVECTOR3(150.f, 10.1f, 50.f));
	dynamic_cast<CHouse*>(pGameObject)->SetHouseColor(1);
	pLayer->AddObject(L"Battle_House", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIROMENT, pLayer));

	return S_OK;
}

HRESULT CBattleGround::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_GAMELOGIC))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_GAMELOGIC];
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	// �÷��̾�
	pGameObject = CBattleGroundPlayer::Create(m_pDevice);
	SetPlayerStartPos(pGameObject);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObjectFront(L"Player", pGameObject);

	// Ŭ���̾�Ʈ ����
	dynamic_cast<CBattleGroundPlayer*>(pGameObject)->SetClient();
	// �ε��� ����
	pGameObject->SetServerIndex(g_iClientIndex); // ���� ������ �� �޾Ƴ��� �ε���

	SetPlayerStartPos(pGameObject);

	//cout << endl << " - " << g_iClientIndex + 1 << " �� Ŭ���̾�Ʈ" << endl << endl;
	
	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent(L"Transform");

	Engine::CGameObject*		pCubeFoot = NULL;

	pCubeFoot = CCubePlayerLeftFoot::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pCubeFoot, E_FAIL);
	dynamic_cast<CCubePlayerLeftFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CBattleGroundPlayer*>(pGameObject)->GetPlayerState()));
	pLayer->AddObject(L"PlayerLeftFoot", pCubeFoot);

	pCubeFoot = CCubePlayerRightFoot::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pCubeFoot, E_FAIL);
	dynamic_cast<CCubePlayerRightFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CBattleGroundPlayer*>(pGameObject)->GetPlayerState()));
	pLayer->AddObject(L"PlayerRightFoot", pCubeFoot);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CBattleGround::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = nullptr;
	if (m_mapLayer.end() == m_mapLayer.find(LAYER_UI))
		pLayer = Engine::CLayer::Create();
	else
		pLayer = m_mapLayer[LAYER_UI];
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	// ä��
	pGameObject = CChat::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Chat", pGameObject);

	// PlayerLook Camera
	Engine::CTransform* pTransform = (Engine::CTransform*)m_mapLayer[LAYER_GAMELOGIC]->GetComponent(L"Player", L"Transform");
	pGameObject = CPlayerLookCamera::Create(m_pDevice, pTransform);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"PlayerLookCamera", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));

	return S_OK;
}

void CBattleGround::Release(void)
{
	Engine::Get_InfoSubject()->UnSubscribe(m_pBulletObserver);
	Engine::Get_InfoSubject()->UnSubscribe(m_pItemObserver);
	Engine::Get_InfoSubject()->UnSubscribe(m_pMapObserver);

	Engine::Safe_Delete(m_pItemObserver);
	Engine::Safe_Delete(m_pBulletObserver);
	Engine::Safe_Delete(m_pMapObserver);

	CloseHandle(m_fMeteorHandle);
	CloseHandle(m_fItemHandle);
}

void CBattleGround::SetPlayerStartPos(Engine::CGameObject * pObj)
{
	switch (g_iClientIndex)
	{
	case 0:
		pObj->SetPos(D3DXVECTOR3(0.f, 1.5f, 0.f));
		break;
	case 1:
		pObj->SetPos(D3DXVECTOR3(200.f, 1.5f, 0.f));
		break;
	case 2:
		pObj->SetPos(D3DXVECTOR3(0.f, 1.5f, 200.f));
		break;
	case 3:
		pObj->SetPos(D3DXVECTOR3(200.f, 1.5f, 200.f));
		break;
	}
}

void CBattleGround::FlagFuntion(void)
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

	if (m_pbSceneFlagArr[FLAG_BULLET_CREATE])
	{
		BulletCreate();
		m_pbSceneFlagArr[FLAG_BULLET_CREATE] = false;
	}
}

void CBattleGround::PlayerCreate(void)
{
	// ���� ����Ǿ� �ִ� Ŭ���̾�Ʈ�� ������ �����´�.
	/*if (m_pNet->GetMapNetData(WORD(FLAG_PLAYER_CREATE)) == nullptr)
		return;*/

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
			Engine::CGameObject*	pObj = CBattleGroundPlayer::Create(m_pDevice);
			pObj->SetServerIndex(i);
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(_T("Player"), pObj);

			//cout << i + 1 << "�� �÷��̾� ����" << endl;

			Engine::CTransform* pTransform = (Engine::CTransform*)pObj->GetComponent(L"Transform");

			Engine::CGameObject*		pCubeFoot = NULL;

			pCubeFoot = CCubePlayerLeftFoot::Create(m_pDevice, pTransform);
			dynamic_cast<CCubePlayerLeftFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CBattleGroundPlayer*>(pObj)->GetPlayerState()));
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(L"PlayerLeftFoot", pCubeFoot);

			pCubeFoot = CCubePlayerRightFoot::Create(m_pDevice, pTransform);
			dynamic_cast<CCubePlayerRightFoot*>(pCubeFoot)->SetPlayerState((dynamic_cast<CBattleGroundPlayer*>(pObj)->GetPlayerState()));
			m_mapLayer[LAYER_GAMELOGIC]->AddObject(L"PlayerRightFoot", pCubeFoot);
		}
	}

	++m_iPlayerCreateCnt;

}

void CBattleGround::PlayerUpdate(void)
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


void CBattleGround::BulletCreate(void)
{
	// � �÷��̾ ���� ����
	m_iBulletCreatePlayerIndex = ((Engine::NETDATA*)(m_pNet->GetMapNetData(WORD(FLAG_BULLET_CREATE))))->iIndex;
	Engine::Get_InfoSubject()->Notify(MESSAGE_BULLET_PLAYER_INDEX);
	Engine::Get_InfoSubject()->Notify(MESSAGE_BULLET_CREATE);
}

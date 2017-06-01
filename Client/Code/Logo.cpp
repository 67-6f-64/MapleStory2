#include "stdafx.h"
#include "Logo.h"

#include "Include.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Layer.h"
#include "GameObject.h"
#include "LogoBack.h"
#include "KeyMgr.h"

#include "ClientNetwork.h"
#include "Loading.h"
#include "GameStart.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
, m_pNet(NETMGR)
, m_bLodeFinish(false)
{

}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::Initialize(void)
{
	HRESULT		hr = NULL;

	Engine::Get_SoundMgr()->PlayBGM(L"MS2_Login_LOOPx1.mp3", Engine::CHANNEL_LOGO);
	
	/////////////////////////////////////////////////////////
	// 네트워크에 레이어 세팅
	m_pNet->SetSceneChange(&m_bSceneChange);
	m_pNet->SetSceneType(&m_wSceneType);
	/////////////////////////////////////////////////////////

	// 서브 쓰레드를 돌기 전에 LogoBack을 출력하자.
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC, Engine::TEXTURE_NORMAL,
		L"Texture_LogoBack", L"../bin/Texture/LogoBack/LogoBack_%d.png", 1);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC, Engine::TEXTURE_NORMAL
		, L"Loading_UI", L"../bin/Texture/Loading/Loading%d.png", 11);
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_DYNAMIC, Engine::TEXTURE_NORMAL
		, L"Start_UI", L"../bin/Texture/Loading/GameStart%d.png", 2);

	Add_GameLogic_Layer();
	Add_UI_Layer();

	LoadThread();
	//std::packaged_task<void()> task(bind(&CLogo::LoadThread, this));
	//thread temp(move(task));
	//temp.detach();
	
	return S_OK;
}

void CLogo::Update(void)
{
	Engine::CScene::Update();

	// TSW 씬 변경 수정
	if (CKeyMgr::GetInstance()->OnceKeyDown(DIK_NUMPADENTER) || CKeyMgr::GetInstance()->OnceKeyDown(DIK_RETURN))
	{
		// 여기서 서버에 현재 바꿀 씬이 어떤 씬인지 알려달라고 요청
		// 디폴트 : SCENE_STAGE(1)
		Engine::NETDATA	tData;
		tData.chKey = 72;
		tData.eMessage = Engine::NETMSG_CLIENT_SCENE_CHECK;
		m_pNet->SendMsg(tData);
	}
}

void CLogo::Render(void)
{
	if (CLoading::GetCount() > -1) 
	{
		if (m_bSceneChange && m_bLodeFinish) // Render에 있는 이유 : Stage의 Update부터 시작하기 위해
		{
			//cout << "로고 - 씬 변경" << endl;
			m_pNet->SetSceneFlagArrNull();
			Engine::Get_Management()->SceneChange(CSceneSelector((SCENEID)m_wSceneType), m_pDevice);
			return;
		}
	}

	Engine::CScene::Render();
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo*	pScene = new CLogo(pDevice);
	if (FAILED(pScene->Initialize()))
		Safe_Delete(pScene);

	return pScene;
}

HRESULT CLogo::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CLogoBack::Create(m_pDevice);
	pLayer->AddObject(L"LogoBack", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CLogo::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CLoading::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Loading", pGameObject);

	pGameObject = CGameStart::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Start", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CLogo::Release(void)
{
	Engine::Get_ResourceMgr()->ResetDynamic();
}

HRESULT CLogo::Add_Buffer(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_TRICOLOR, L"Buffer_TriColor");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCCOLOR, L"Buffer_RcColor");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_RCTEXXZ, L"Buffer_RcTexXZ");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_CUBETEX, L"Buffer_CubeTex");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_SKYCUBETEX, L"Buffer_SkyCubeTex");
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_TERRAINTEX, L"Buffer_TerrainTex", VTXCNTX, VTXCNTY, VTXCNTZ, VTXITV, D3DXVECTOR3(0.f, 1.f, 0.f));
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_WALLTEX, L"Buffer_LeftWallTex", 0, 8, 9, VTXITV, D3DXVECTOR3(1.f, 0.f, 0.f));
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_WALLTEX, L"Buffer_RightWallTex", 0, 8, 9, VTXITV, D3DXVECTOR3(-1.f, 0.f, 0.f));
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_WALLXYTEX, L"Buffer_FrontWallTex", 9, 8, 0, VTXITV, D3DXVECTOR3(0.f, 0.f, -1.f));
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_WALLXYTEX, L"Buffer_BackWallTex", 9, 8, 0, VTXITV, D3DXVECTOR3(0.f, 0.f, 1.f));
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_TERRAINTEX, L"Buffer_UnderWallTex", VTXCNTX, VTXCNTY, VTXCNTZ, VTXITV, D3DXVECTOR3(0.f, 1.f, 0.f));
	FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Engine::RESOURCE_STATIC,
		Engine::BUFFER_TERRAINTEX, L"Buffer_CeilingTex", 50, VTXCNTY, 700, VTXITV, D3DXVECTOR3(0.f, -1.f, 0.f));
	FAILED_CHECK(hr);
		
	return S_OK;
}

HRESULT CLogo::Add_Texture(void)
{
	HRESULT		hr = NULL;

	Add_Texture_Map();
	Add_Texture_Player();
	Add_Textrue_UI();
	Add_Textrue_ETC();

	return S_OK;
}

HRESULT CLogo::Add_Texture_Map(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Terrain", L"../bin/Texture/Terrain/Terrain%d.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Texture_Cube", L"../bin/Texture/Tile/Tile%d.dds", 29); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Floor", L"../bin/Texture/Map/Floor%d.png", 3); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Wall", L"../bin/Texture/Map/Wall%d.png", 2); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Ceiling", L"../bin/Texture/Map/Ceiling%d.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_MazeWall", L"../bin/Texture/Map/MazeWall%d.png", 1); FAILED_CHECK(hr);

	// 총알 궤적용 이미지
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Trail", L"../bin/Texture/Bullet/BulletTrail%d.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_BattleFloor", L"../bin/Texture/Map/BattleFloor%d.jpg", 1); FAILED_CHECK(hr);

	// 배경
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Background_Stage", L"../bin/Texture/StageBackground.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Background_PartyRoom", L"../bin/Texture/PartyRoomBackground.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Background_PartyFall", L"../bin/Texture/BackGroundPartyFall.png", 1); FAILED_CHECK(hr);

	// SkyBox
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Texture_SkyBox", L"../bin/Texture/SkyBox/SkyBox%d.dds", 3); FAILED_CHECK(hr);
	
	// StartFlag (미로 깃발)
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_StartFlag", L"../bin/Texture/Flag/StartFlag/Flag%d.png", 8); FAILED_CHECK(hr);

	// EndFlag
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_EndFlag", L"../bin/Texture/Flag/EndFlag/EndFlag%d.png", 8); FAILED_CHECK(hr);

	// House
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Texture_House", L"../bin/Texture/House/House%d.dds", 2); FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CLogo::Add_Texture_Player(void)
{
	HRESULT		hr = NULL;

	// 플레이어
	// Stand
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Stand_Dir_B", L"../bin/Texture/Player/Stand/Dir_B/Dir_B%d.png", 10); FAILED_CHECK(hr);	
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Stand_Dir_R", L"../bin/Texture/Player/Stand/Dir_R/Dir_R%d.png", 10); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Stand_Dir_RB", L"../bin/Texture/Player/Stand/Dir_RB/Dir_RB%d.png", 10); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Stand_Dir_RT", L"../bin/Texture/Player/Stand/Dir_RT/Dir_RT%d.png", 10); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Stand_Dir_T", L"../bin/Texture/Player/Stand/Dir_T/Dir_T%d.png", 10); FAILED_CHECK(hr);

	// Walk
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Walk_Dir_B", L"../bin/Texture/Player/Walk/Dir_B/Dir_B%d.png", 12); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Walk_Dir_R", L"../bin/Texture/Player/Walk/Dir_R/Dir_R%d.png", 12); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Walk_Dir_RB", L"../bin/Texture/Player/Walk/Dir_RB/Dir_RB%d.png", 12); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Walk_Dir_RT", L"../bin/Texture/Player/Walk/Dir_RT/Dir_RT%d.png", 12); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Walk_Dir_T", L"../bin/Texture/Player/Walk/Dir_T/Dir_T%d.png", 12); FAILED_CHECK(hr);

	// Jump
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Jump_Dir_B", L"../bin/Texture/Player/Jump/Dir_B/Dir_B%d.png", 15); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Jump_Dir_R", L"../bin/Texture/Player/Jump/Dir_R/Dir_R%d.png", 15); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Jump_Dir_RB", L"../bin/Texture/Player/Jump/Dir_RB/Dir_RB%d.png", 15); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Jump_Dir_RT", L"../bin/Texture/Player/Jump/Dir_RT/Dir_RT%d.png", 15); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Player_Jump_Dir_T", L"../bin/Texture/Player/Jump/Dir_T/Dir_T%d.png", 15); FAILED_CHECK(hr);

	// 캐릭터 큐브
	/*hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Player_Cat", L"../bin/Texture/CubePlayer/Cat%d.dds", 4); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Player_Dark", L"../bin/Texture/CubePlayer/Dark%d.dds", 4); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Player_Man", L"../bin/Texture/CubePlayer/Man%d.dds", 4); FAILED_CHECK(hr);
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Player_Chicken", L"../bin/Texture/CubePlayer/Chicken%d.dds", 4); FAILED_CHECK(hr);*/
	
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Player_Cube", L"../bin/Texture/CubePlayer/%d.dds", 4); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Player_Foot", L"../bin/Texture/CubePlayer/Foot/Foot%d.dds", 1); FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CLogo::Add_Textrue_UI(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_MainUI", L"../bin/Texture/UI/MainUI.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_HPMPGauge", L"../bin/Texture/UI/HPMPGauge/HPMPGauge.png", 1); FAILED_CHECK(hr);
	
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_UINumber", L"../bin/Texture/UI/Number/Number%d.png", 4); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_PlayerWin", L"../bin/Texture/UI/PlayerWin/PlayerWin%d.png", 4); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Chat", L"../bin/Texture/UI/Chat/Chat%d.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_ChatUI", L"../bin/Texture/UI/Chat/ChatUI%d.png", 1); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Portal_UI", L"../bin/Texture/UI/Portal/0_%d.png", 6); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_LowHP", L"../bin/Texture/UI/LowHp/LowHp%d.png", 1); FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CLogo::Add_Textrue_Effect(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Meteor_Move", L"../bin/Texture/Effect/Meteor/Move/Move%d.png", 3); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Meteor_Attack", L"../bin/Texture/Effect/Meteor/Attack/Attack%d.png", 16); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Meteor", L"../bin/Texture/Effect/Meteor/Meteor/FireBolt%d.png", 4); FAILED_CHECK(hr);
	return S_OK;
}

HRESULT CLogo::Add_Textrue_ETC(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_CUBE
		, L"Texture_CubeItem", L"../bin/Texture/Item/ItemBox.dds", 1); FAILED_CHECK(hr);

	// Portal
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"PartyRoom_Portal_To_PartyFall", L"../bin/Texture/Portal/PartyRoomPortal0/0_%d.png", 40); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"PartyRoom_Portal_To_PartyMaze", L"../bin/Texture/Portal/PartyRoomPortal1/%d.png", 8); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"PartyRoom_Portal_To_BattleGround", L"../bin/Texture/Portal/PartyRoomPortal2/1_%d.png", 14); FAILED_CHECK(hr);

	// Item (포션, 
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Portion", L"../bin/Texture/Portion%d.png", 1); FAILED_CHECK(hr);

	// Bullet
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Bullet", L"../bin/Texture/Bullet/PShield/PShield%d.png", 7); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"Texture_Bullet2", L"../bin/Texture/Bead/%d.png", 16); FAILED_CHECK(hr);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Engine::RESOURCE_STATIC, Engine::TEXTURE_NORMAL
		, L"PlayerHit", L"../bin/Texture/Effect/PlayerHit/%d.png", 4); FAILED_CHECK(hr);

	return S_OK;
}

void CLogo::LoadThread(void)
{
	HRESULT hr;

	hr = Add_Buffer();
	hr = Add_Texture();
	hr = Add_Textrue_Effect();

	hr = Add_Environment_Layer();	
	
	m_bLodeFinish = true;
}


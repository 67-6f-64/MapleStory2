/*!
 * \file Engine_Enum.h
 * \date 2015/05/01 15:57
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

#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	//! @ brief 씬이 전환될때 지워야할 리소스와 유지해야할 리소스로 나눈다.
	enum RESOURCETYPE
	{
		RESOURCE_STATIC,	//!< 유지해야 할 리소스
		RESOURCE_DYNAMIC,	//!< 지워야 할 리소스
		RESOURCE_END,
	};

	enum TEXTURETYPE
	{
		TEXTURE_NORMAL,
		TEXTURE_CUBE,
		TEXTURE_END,
	};

	enum BUFFERTYPE
	{
		BUFFER_TRICOLOR,
		BUFFER_RCCOLOR,
		BUFFER_RCTEX,
		BUFFER_RCTEXXZ,
		BUFFER_CUBETEX,
		BUFFER_CUBECOLOR,
		BUFFER_SKYCUBETEX,
		BUFFER_TERRAINTEX,
		BUFFER_WALLTEX,
		BUFFER_WALLXYTEX,
	};

	enum ANGLE {ANGLE_X, ANGLE_Y, ANGLE_Z, ANGLE_END,};

	//TSW 추가 서버-클라 통신용 메세지 추가
	enum eNETMSG
	{
		NETMSG_SERVER_SET_INDEX,			// 플레이어 생성할때 인덱스 세팅용(서버 전용)

		// 플레이어 메세지
		NETMSG_SERVER_CREATE_PLAYER,		// 플레이어 생성메세지(서버전용)
		NETMSG_SERVER_DELETE_PLAYER,		// 플레이어 접속 종료시 플레이어 삭제(서버 전용)

		NETMSG_SERVER_PLAYER_UPDATE,		// 클라이언트로부터 받은 상태를 다른 클라로 업데이트해주는 메세지(서버 전용)
		NETMSG_CLIENT_CREATE_PLAYER,		// 해당 클라이언트외의 다른 클라이언트의 플레이어를 생성하기 위해 서버에 요청(클라이언트 전용)
		NETMSG_CLIENT_PLAYER_NOTIPY,		// 플레이어 상태변경 게시(클라이언트 전용)

		// 불렛 메세지
		NETMSG_SERVER_BULLET_CREATE,		// 불렛 생성 메세지(서버 전용)
		NETMSG_CLIENT_BULLET_CREATE_NOTIPY,	// 불렛 생성 메세지(클라이언트 전용)

		// 씬 확인용 메세지 - 로고에서 사용
		NETMSG_CLIENT_SCENE_CHECK,			// 클라이언트 시작시 서버에 현재 씬이 어떤 씬인지 확인(클라이언트 전용)

		// 씬 변경용 메세지
		NETMSG_CLIENT_SCENE_CHANGE,		// 플레이어 파티장(아무나)이 씬 변경 요청메세지를 발송(클라이언트 전용)
		NETMSG_SERVER_SCENE_CHANGE,		// 클라에서 씬 변경 요청이 오면 모든 클라이언트에게 씬변경 메세지를 발송(서버 전용)

		// 채팅용 메세지
		NETMSG_CLIENT_SEND_CHATTING,	// 채팅 생성 메세지(클라이언트 전용)
		NETMSG_SERVER_SEND_CHATTING,	// 채팅 생성 메세지(서버 전용)

		// ----- PartyFall -------
		// 큐브 낙하용 메세지
		NETMSG_CLIENT_PARTYFALL_CUBE,		// 큐브 낙하 인덱스정보를 넘겨주는 메세지(클라이언트 전용)
		NETMSG_SERVER_PARTYFALL_CUBE,		// 큐브 낙하 인덱스정보를 전체 클라이언트에 넘겨주는 메세지(서버 전용)

		// 아이템 충돌용 메세지
		NETMSG_CLIENT_PARTYFALL_ITEM,		// 아이템 충돌시 서버로 넘겨주는 메세지(클라이언트 전용)
		NETMSG_SERVER_PARTYFALL_ITEM,		// 받은 아이템의 인덱스를 전체 클라이언트에 넘겨주는 메세지(서버 전용)

		NETMSG_END
	};

	enum CHANNEL_TYPE {
		// 스테이지 브금
		CHANNEL_LOGO, CHANNEL_STAGE, CHANNEL_PARTYROOM, CHANNEL_PARTYFALL, CHANNEL_PARTYMAZE, CHANNEL_BATTLEGROUND,
		CHANNEL_PARTYROOM_COLLAPSE,	CHANNEL_PARTYMAZE_THUNDER,

		CHANNEL_PLAYER_PISTOL, CHANNEL_PLAYER_LOWHP,
		CHANNEL_METEOR_FALL, CHANNEL_METEOR_HIT,
		CHANNEL_ITEM_DRINK,
		CHANNEL_END
	};

	// 기본 플래그 - 추가가능 ( 씬마다 필요한 플래그가 다르므로 상수를 초기화 해야함 예: flag = 상수)
	enum eFLAGID {
		FLAG_PLAYER_CREATE = 0,
		FLAG_PLAYER_UPDATE = 1,
		FLAG_BULLET_CREATE = 2,
		FLAG_END };
}

#endif // Engine_Enum_h__
#ifndef Enum_h__
#define Enum_h__

enum SCENEID { SCENE_LOGO, SCENE_STAGE, SCENE_PARTYROOM, SCENE_END };

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
	NETMSG_CLIENT_SCENE_CHANGE,			// 플레이어 파티장(아무나)이 씬 변경 요청메세지를 발송(클라이언트 전용)
	NETMSG_SERVER_SCENE_CHANGE,			// 클라에서 씬 변경 요청이 오면 모든 클라이언트에게 씬변경 메세지를 발송(서버 전용)

	// 채팅용 메세지
	NETMSG_CLIENT_SEND_CHATTING,		// 채팅 생성 메세지(클라이언트 전용)
	NETMSG_SERVER_SEND_CHATTING,		// 채팅 생성 메세지(서버 전용)

	// ----- PartyFall -------
	// 큐브 낙하용 메세지
	NETMSG_CLIENT_PARTYFALL_CUBE,		// 큐브 낙하 인덱스정보를 넘겨주는 메세지(클라이언트 전용)
	NETMSG_SERVER_PARTYFALL_CUBE,		// 큐브 낙하 인덱스정보를 전체 클라이언트에 넘겨주는 메세지(서버 전용)

	// 아이템 충돌용 메세지
	NETMSG_CLIENT_PARTYFALL_ITEM,		// 아이템 충돌시 서버로 넘겨주는 메세지(클라이언트 전용)
	NETMSG_SERVER_PARTYFALL_ITEM,		// 받은 아이템의 인덱스를 전체 클라이언트에 넘겨주는 메세지(서버 전용)

	NETMSG_END
};

#endif // Enum_h__
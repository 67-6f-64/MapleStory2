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
	//! @ brief ���� ��ȯ�ɶ� �������� ���ҽ��� �����ؾ��� ���ҽ��� ������.
	enum RESOURCETYPE
	{
		RESOURCE_STATIC,	//!< �����ؾ� �� ���ҽ�
		RESOURCE_DYNAMIC,	//!< ������ �� ���ҽ�
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

	//TSW �߰� ����-Ŭ�� ��ſ� �޼��� �߰�
	enum eNETMSG
	{
		NETMSG_SERVER_SET_INDEX,			// �÷��̾� �����Ҷ� �ε��� ���ÿ�(���� ����)

		// �÷��̾� �޼���
		NETMSG_SERVER_CREATE_PLAYER,		// �÷��̾� �����޼���(��������)
		NETMSG_SERVER_DELETE_PLAYER,		// �÷��̾� ���� ����� �÷��̾� ����(���� ����)

		NETMSG_SERVER_PLAYER_UPDATE,		// Ŭ���̾�Ʈ�κ��� ���� ���¸� �ٸ� Ŭ��� ������Ʈ���ִ� �޼���(���� ����)
		NETMSG_CLIENT_CREATE_PLAYER,		// �ش� Ŭ���̾�Ʈ���� �ٸ� Ŭ���̾�Ʈ�� �÷��̾ �����ϱ� ���� ������ ��û(Ŭ���̾�Ʈ ����)
		NETMSG_CLIENT_PLAYER_NOTIPY,		// �÷��̾� ���º��� �Խ�(Ŭ���̾�Ʈ ����)

		// �ҷ� �޼���
		NETMSG_SERVER_BULLET_CREATE,		// �ҷ� ���� �޼���(���� ����)
		NETMSG_CLIENT_BULLET_CREATE_NOTIPY,	// �ҷ� ���� �޼���(Ŭ���̾�Ʈ ����)

		// �� Ȯ�ο� �޼��� - �ΰ��� ���
		NETMSG_CLIENT_SCENE_CHECK,			// Ŭ���̾�Ʈ ���۽� ������ ���� ���� � ������ Ȯ��(Ŭ���̾�Ʈ ����)

		// �� ����� �޼���
		NETMSG_CLIENT_SCENE_CHANGE,		// �÷��̾� ��Ƽ��(�ƹ���)�� �� ���� ��û�޼����� �߼�(Ŭ���̾�Ʈ ����)
		NETMSG_SERVER_SCENE_CHANGE,		// Ŭ�󿡼� �� ���� ��û�� ���� ��� Ŭ���̾�Ʈ���� ������ �޼����� �߼�(���� ����)

		// ä�ÿ� �޼���
		NETMSG_CLIENT_SEND_CHATTING,	// ä�� ���� �޼���(Ŭ���̾�Ʈ ����)
		NETMSG_SERVER_SEND_CHATTING,	// ä�� ���� �޼���(���� ����)

		// ----- PartyFall -------
		// ť�� ���Ͽ� �޼���
		NETMSG_CLIENT_PARTYFALL_CUBE,		// ť�� ���� �ε��������� �Ѱ��ִ� �޼���(Ŭ���̾�Ʈ ����)
		NETMSG_SERVER_PARTYFALL_CUBE,		// ť�� ���� �ε��������� ��ü Ŭ���̾�Ʈ�� �Ѱ��ִ� �޼���(���� ����)

		// ������ �浹�� �޼���
		NETMSG_CLIENT_PARTYFALL_ITEM,		// ������ �浹�� ������ �Ѱ��ִ� �޼���(Ŭ���̾�Ʈ ����)
		NETMSG_SERVER_PARTYFALL_ITEM,		// ���� �������� �ε����� ��ü Ŭ���̾�Ʈ�� �Ѱ��ִ� �޼���(���� ����)

		NETMSG_END
	};

	enum CHANNEL_TYPE {
		// �������� ���
		CHANNEL_LOGO, CHANNEL_STAGE, CHANNEL_PARTYROOM, CHANNEL_PARTYFALL, CHANNEL_PARTYMAZE, CHANNEL_BATTLEGROUND,
		CHANNEL_PARTYROOM_COLLAPSE,	CHANNEL_PARTYMAZE_THUNDER,

		CHANNEL_PLAYER_PISTOL, CHANNEL_PLAYER_LOWHP,
		CHANNEL_METEOR_FALL, CHANNEL_METEOR_HIT,
		CHANNEL_ITEM_DRINK,
		CHANNEL_END
	};

	// �⺻ �÷��� - �߰����� ( ������ �ʿ��� �÷��װ� �ٸ��Ƿ� ����� �ʱ�ȭ �ؾ��� ��: flag = ���)
	enum eFLAGID {
		FLAG_PLAYER_CREATE = 0,
		FLAG_PLAYER_UPDATE = 1,
		FLAG_BULLET_CREATE = 2,
		FLAG_END };
}

#endif // Engine_Enum_h__
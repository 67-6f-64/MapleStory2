#ifndef Enum_h__
#define Enum_h__

enum SCENEID { SCENE_LOGO, SCENE_STAGE, SCENE_PARTYROOM, SCENE_END };

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
	NETMSG_CLIENT_SCENE_CHANGE,			// �÷��̾� ��Ƽ��(�ƹ���)�� �� ���� ��û�޼����� �߼�(Ŭ���̾�Ʈ ����)
	NETMSG_SERVER_SCENE_CHANGE,			// Ŭ�󿡼� �� ���� ��û�� ���� ��� Ŭ���̾�Ʈ���� ������ �޼����� �߼�(���� ����)

	// ä�ÿ� �޼���
	NETMSG_CLIENT_SEND_CHATTING,		// ä�� ���� �޼���(Ŭ���̾�Ʈ ����)
	NETMSG_SERVER_SEND_CHATTING,		// ä�� ���� �޼���(���� ����)

	// ----- PartyFall -------
	// ť�� ���Ͽ� �޼���
	NETMSG_CLIENT_PARTYFALL_CUBE,		// ť�� ���� �ε��������� �Ѱ��ִ� �޼���(Ŭ���̾�Ʈ ����)
	NETMSG_SERVER_PARTYFALL_CUBE,		// ť�� ���� �ε��������� ��ü Ŭ���̾�Ʈ�� �Ѱ��ִ� �޼���(���� ����)

	// ������ �浹�� �޼���
	NETMSG_CLIENT_PARTYFALL_ITEM,		// ������ �浹�� ������ �Ѱ��ִ� �޼���(Ŭ���̾�Ʈ ����)
	NETMSG_SERVER_PARTYFALL_ITEM,		// ���� �������� �ε����� ��ü Ŭ���̾�Ʈ�� �Ѱ��ִ� �޼���(���� ����)

	NETMSG_END
};

#endif // Enum_h__
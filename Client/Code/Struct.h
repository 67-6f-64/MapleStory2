#ifndef Struct_h__
#define Struct_h__

// ť�� ������
typedef struct tagCubeData
{
	DWORD				dwTextureNum;		// ť�� �ؽ���
	D3DXVECTOR3			vPos;				// ��ġ
	eCUBESEQUENCE		eCubeSequence;		// ���� ���� 
	bool				bOutSide;			// ��, ��
}CUBEDATA;

typedef struct tagCamera
{
	D3DXVECTOR3 vecEye;
	D3DXVECTOR3 vecDir;
}CAM_INFO;

typedef struct tagWallData
{
	D3DXVECTOR3         vPos;         // ���� ��ġ
	WALL_DIR            eWallDir;      // ���� ����
}WALLDATA;

typedef struct tagLightData
{
	D3DXVECTOR3         vPos;               // ������ ��ġ
	int                 iLightCount;         // ���� �ε��� 
}LIGHTDATA;

typedef struct tagPlayerData
{
	D3DXVECTOR3* vecPos;
	float*		 m_fSpeed;
}PLAYER_DATA;

typedef struct tagPartyFallPlayerData
{
	D3DXVECTOR3* vecPos;
	int*		 pIndex;
}PARTYFALL_PLAYER_DATA;

typedef struct tagBulletData
{
	tagBulletData(void)
	{}

	tagBulletData(D3DXVECTOR3 _vecPos, D3DXVECTOR3 _vecDir, eBULLETID _eBullet, int _iIndex)
		:vecPos(_vecPos), vecDir(_vecDir), eBullet(_eBullet), iIndex(_iIndex)
	{}

	D3DXVECTOR3 vecPos;
	D3DXVECTOR3 vecDir;
	eBULLETID	eBullet;
	int			iIndex;

}BULLET_DATA;

typedef struct tagBattlePlayer
{
	tagBattlePlayer(void)
	{}

	tagBattlePlayer(D3DXVECTOR3 _vecPos, int _iLife, int _iServerIndex)
		:vecPos(_vecPos), iLife(_iLife), iServerIndex(_iServerIndex)
	{}

	D3DXVECTOR3 vecPos;
	int			iLife;
	int			iServerIndex;

	// Gun Type �� �߰�

}BATTLE_PLAYER_DATA;


#endif // Struct_h__

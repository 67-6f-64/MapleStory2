#ifndef Struct_h__
#define Struct_h__

// 큐브 데이터
typedef struct tagCubeData
{
	DWORD				dwTextureNum;		// 큐브 텍스쳐
	D3DXVECTOR3			vPos;				// 위치
	eCUBESEQUENCE		eCubeSequence;		// 렌더 순서 
	bool				bOutSide;			// 안, 밖
}CUBEDATA;

typedef struct tagCamera
{
	D3DXVECTOR3 vecEye;
	D3DXVECTOR3 vecDir;
}CAM_INFO;

typedef struct tagWallData
{
	D3DXVECTOR3         vPos;         // 벽의 위치
	WALL_DIR            eWallDir;      // 벽의 방향
}WALLDATA;

typedef struct tagLightData
{
	D3DXVECTOR3         vPos;               // 조명의 위치
	int                 iLightCount;         // 조명 인덱스 
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

	// Gun Type 등 추가

}BATTLE_PLAYER_DATA;


#endif // Struct_h__

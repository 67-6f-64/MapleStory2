#ifndef Struct_h__
#define Struct_h__

typedef struct tagNetData
{
	char		chKey;
	eNETMSG		eMessage;		// 주 메세지
	WORD		wSubMessage;	// 서브 메세지
	int			iIndex;
	D3DXMATRIX  matWold;
	DWORD		dwFrame;
	int         iTexture;
	char		szMessage[16];
}NETDATA, *PNETDATA, *pNETDATA;

#endif // Struct_h__
#ifndef Struct_h__
#define Struct_h__

typedef struct tagNetData
{
	char		chKey;
	eNETMSG		eMessage;		// �� �޼���
	WORD		wSubMessage;	// ���� �޼���
	int			iIndex;
	D3DXMATRIX  matWold;
	DWORD		dwFrame;
	int         iTexture;
	char		szMessage[16];
}NETDATA, *PNETDATA, *pNETDATA;

#endif // Struct_h__
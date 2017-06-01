/*!
 * \file Engine_Struct.h
 * \date 2015/04/30 16:06
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

#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagSringCmp
	{
		const TCHAR* m_pString;

		template<class T>
		bool operator () (T data)
		{
			return !lstrcmp(data.first, m_pString);
		}

		tagSringCmp(const TCHAR* pKey)
			:m_pString(pKey) {}

	}STRING_CMP;

	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;
	}VTXCOL;
	const DWORD		VTXCOL_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;
	const DWORD		VTXTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;


	typedef struct tagLightVertex
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexUV;
	}VTXLIGHT;
	const DWORD		VTXLIGHT_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;
	}VTXCUBE;
	const DWORD		VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;

	typedef struct tagInfo
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vDir;
	}INFO;

	typedef struct tagFrame
	{
		float fFrame;
		float fCount;
		float fMax;

		tagFrame(float _fFrame = 0, float _fCount = 0, float _fMax = 0)
			:fFrame(_fFrame)
			, fCount(_fCount)
			, fMax(_fMax)
		{}
	}FRAME;

	//TSW 추가 서버-클라 통신용 구조체 추가
	typedef struct tagNetData
	{
		char		chKey;
		eNETMSG		eMessage;		// 주 메세지
		WORD		wSubMessage;	// 서브 메세지
		int			iIndex;
		D3DXMATRIX  matWold;
		DWORD		dwFrame;
		int         iTexture;
		CHAR		szMessage[16];

	}NETDATA, *PNETDATA, *pNETDATA;
}

#endif // Engine_Struct_h__
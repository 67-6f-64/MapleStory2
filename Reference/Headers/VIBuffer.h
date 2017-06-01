/*!
 * \file VIBuffer.h
 * \date 2017/04/26 15:55
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

#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer
	: public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CVIBuffer(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	virtual CResources* CloneResource(void) PURE;
	void GetVtxInfo(void* pVertex);
	void SetVtxInfo(void* pVertex);

public:
	virtual void Render(void);

protected:
	void Release(void);

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	DWORD		m_dwVtxSize;
	DWORD		m_dwVtxCnt;
	DWORD		m_dwVtxFVF;

protected:
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	DWORD		m_dwIdxSize;
	DWORD		m_dwTriCnt;
	D3DFORMAT	m_IdxFmt;

};

END

#endif // VIBuffer_h__
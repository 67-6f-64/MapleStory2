/*!
* \file InfoSubject.h
* \date 2017/05/10 15:51
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

#ifndef InfoSubject_h__
#define InfoSubject_h__

#include "Subject.h"

BEGIN(Engine)

typedef list<void*>				DATALIST;
typedef map<int, DATALIST>		MAPDATALIST;

class ENGINE_DLL CInfoSubject
	: public CSubject
{
public:
	DECLARE_SINGLETON(CInfoSubject)

private:
	CInfoSubject(void);
	virtual ~CInfoSubject(void);

public:
	const list<void*>* GetDatalist(int message);

public:
	void AddData(int message, void* pData);
	void AddDataFront(int message, void* pData);

	void RemoveData(int message, void* pData);

private:
	void Release(void);

private:
	MAPDATALIST		m_mapDatalist;
};

END

#endif // InfoSubject_h__
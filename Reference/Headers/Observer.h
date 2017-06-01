/*!
* \file Observer.h
* \date 2017/05/10 15:33
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

#ifndef Observer_h__
#define Observer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CObserver
{
protected:
	CObserver(void);

public:
	virtual ~CObserver(void);

public:
	virtual void Update(int message) PURE;
};

END

#endif // Observer_h__
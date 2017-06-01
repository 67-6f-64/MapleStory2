#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CComponent
{
protected :
	CComponent(void);

public:
	virtual ~CComponent(void) = 0;

public:
	virtual void Update(void);

};

END
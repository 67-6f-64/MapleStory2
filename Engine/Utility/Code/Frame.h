
#ifndef Frame_h__
#define Frame_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFrame
	: public CComponent
{
private:
	explicit CFrame(const FRAME tFrame);

public:
	virtual ~CFrame(void);

public:
	virtual void Update(void);

public:
	static CFrame* Create(const FRAME tFrame);

public:
	FRAME m_tFrame;
};

END

#endif // Frame_h__
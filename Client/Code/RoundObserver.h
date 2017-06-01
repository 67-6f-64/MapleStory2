
#ifndef RoundObserver_h__
#define RoundObserver_h__

#include "Observer.h"
#include "Include.h"

class CRoundObserver
	: public Engine::CObserver
{
private:
	CRoundObserver(void);

public:
	virtual ~CRoundObserver(void);

public:
	virtual void Update(int message);

	void PushPlayer(void) { m_VecPlayerFall.push_back(false); }

	int IsOnlyOneSurvive(void);

public:
	static CRoundObserver* Create(void);

private:
	eGameState		m_eGameState;
	vector<bool>	m_VecPlayerFall;
};

#endif // RoundObserver_h__
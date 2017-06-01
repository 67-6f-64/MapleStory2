#include "Subject.h"

#include "Observer.h"

Engine::CSubject::CSubject(void)
{

}

Engine::CSubject::~CSubject(void)
{
	Release();
}

void Engine::CSubject::Subscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);
	m_Observerlist.push_back(pObserver);
}

void Engine::CSubject::UnSubscribe(CObserver* pObserver)
{
	OBSERVERLIST::iterator	iter = m_Observerlist.begin();
	OBSERVERLIST::iterator	iter_end = m_Observerlist.end();

	for (; iter != iter_end; )
	{
		if ((*iter) == pObserver)
		{
			m_Observerlist.erase(iter);
			return;
		}
		else
			++iter;
	}
}

void Engine::CSubject::Notify(int message)
{
	OBSERVERLIST::iterator	iter = m_Observerlist.begin();
	OBSERVERLIST::iterator	iter_end = m_Observerlist.end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Update(message);
	}
}

void Engine::CSubject::Release(void)
{
	m_Observerlist.clear();
}


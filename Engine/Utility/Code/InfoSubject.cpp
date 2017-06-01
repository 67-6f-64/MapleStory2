#include "InfoSubject.h"

IMPLEMENT_SINGLETON(Engine::CInfoSubject)

Engine::CInfoSubject::CInfoSubject(void)
{

}

Engine::CInfoSubject::~CInfoSubject(void)
{
	Release();
}

void Engine::CInfoSubject::AddData(int message, void* pData)
{
	if (pData)
	{
		MAPDATALIST::iterator	iter = m_mapDatalist.find(message);
		if (iter == m_mapDatalist.end())
		{
			m_mapDatalist[message] = DATALIST();
		}
		m_mapDatalist[message].push_back(pData);
		Notify(message);
	}
}

void Engine::CInfoSubject::AddDataFront(int message, void * pData)
{
	if (pData)
	{
		MAPDATALIST::iterator	iter = m_mapDatalist.find(message);
		if (iter == m_mapDatalist.end())
		{
			m_mapDatalist[message] = DATALIST();
		}
		m_mapDatalist[message].push_front(pData);
		Notify(message);
	}
}

void Engine::CInfoSubject::RemoveData(int message, void* pData)
{
	MAPDATALIST::iterator		mapiter = m_mapDatalist.find(message);
	if (mapiter != m_mapDatalist.end())
	{
		DATALIST::iterator		iterlist = mapiter->second.begin();
		DATALIST::iterator		iterlist_end = mapiter->second.end();

		for (; iterlist != iterlist_end; ++iterlist)
		{
			if ((*iterlist) == pData)
			{
				mapiter->second.erase(iterlist);
				return;
			}
		}
	}
}

void Engine::CInfoSubject::Release(void)
{
	for (MAPDATALIST::iterator iter = m_mapDatalist.begin();
		iter != m_mapDatalist.end(); ++iter)
	{
		iter->second.clear();
	}
	m_mapDatalist.clear();
}

const list<void*>* Engine::CInfoSubject::GetDatalist(int message)
{
	MAPDATALIST::iterator	iter = m_mapDatalist.find(message);
	if (iter == m_mapDatalist.end())
		return NULL;

	return &iter->second;
}
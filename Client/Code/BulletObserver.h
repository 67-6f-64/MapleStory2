
#ifndef BulletObserver_h__
#define BulletObserver_h__

#include "Observer.h"
#include "Include.h"

class CBullet;
class CBulletObserver
	: public Engine::CObserver
{
private:
	CBulletObserver(void);

public:
	virtual ~CBulletObserver(void);

public:
	virtual void Update(int message);

	bool CollideCheck(BATTLE_PLAYER_DATA* PlayerData);

	// ÃÑ¾Ë ¾÷µ¥ÀÌÆ®, ·»´õ
	void Update(void);
	void Render(void);

	bool GetShowPlayerHit(void) { return m_bShowPlayerHitUI; }
	void SetShowPlayerHit(bool b) { m_bShowPlayerHitUI = b; }

public:
	static CBulletObserver* Create(void);

private:
	vector<CBullet*>	m_VecBullet;		
	int					m_iBulletCreatePlayerIndex;

	bool				m_bShowPlayerHitUI;
};

#endif // BulletObserver_h__
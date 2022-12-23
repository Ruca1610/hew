#pragma once
#include "CameraBase.h"
#include "Player.h"

class CameraPlayer : public CameraBase
{
public:
	CameraPlayer(Player* pPlayer);
	~CameraPlayer();
	void Update();
private:
	Player* m_pPlayer;
	float m_radY;
	float m_radXZ;
	float m_radius;
	float m_lateXZ;
	float m_lateY;	// –Ú“IˆÊ’u‚É‘Î‚µ‚Ä’x‚ê‚Ä‚Â‚¢‚Ä‚¢‚­Šp“x(Œ»İ‚ÌŠp“x
};
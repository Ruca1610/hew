#include "CameraPlayer.h"
#include "Input.h"

CameraPlayer::CameraPlayer(Player* pPlayer)
	:m_pPlayer(pPlayer)
	, m_radY(-0.5f)
	, m_radXZ(0.0f)
	, m_radius(-5.0f)
	, m_lateXZ(0.0f)
	, m_lateY(0.0f)
{
}

CameraPlayer::~CameraPlayer()
{

}

void CameraPlayer::Update()
{
	m_look = m_pPlayer->GetPos();
	m_look.y += 2.0f;
	if (IsKeyPress(VK_UP))		m_radY -= 0.05f;
	if (IsKeyPress(VK_LEFT))	m_radXZ -= 0.05f;
	if (IsKeyPress(VK_DOWN))	m_radY += 0.05f;
	if (IsKeyPress(VK_RIGHT))	m_radXZ += 0.05f;
	if (90 * 3.14 / 180 < m_radY) {
		m_radY = 90 * 3.14 / 180;
	}
	if (-90 * 3.14 / 180 > m_radY) {
		m_radY = -90 * 3.14 / 180;
	}

	m_lateXZ = (m_radXZ - m_lateXZ) * 0.1f + m_lateXZ;
	m_lateY = (m_radY - m_lateY) * 0.1f + m_lateY	;


	m_pos.x = cos(m_lateY) * sin(m_lateXZ) * m_radius + m_look.x;
	m_pos.y = sin(m_lateY) * m_radius + m_look.y;
	m_pos.z = cos(m_lateY) * cos(m_lateXZ) * m_radius + m_look.z;
}

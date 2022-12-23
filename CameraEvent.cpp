#include "CameraEvent.h"

CameraEvent::CameraEvent()
	:m_start(0.0f,0.0f,0.0f)
	,m_end(0.0f,0.0f,0.0f)
	,m_time(0.0f)
	,m_totalTime(0.0f)
{

}

CameraEvent::~CameraEvent()
{

}

void CameraEvent::Update()
{
	if (!IsEvent())
	{
		return;
	}
	m_look.x = (m_end.x - m_start.x) * -(cos(3.14f * (m_time / m_totalTime) - 1 / 2)) + m_start.x;
	m_look.y = (m_end.y - m_start.y) * -(cos(3.14f * (m_time / m_totalTime) - 1 / 2)) + m_start.y;
	m_look.z = (m_end.z - m_start.z) * -(cos(3.14f * (m_time / m_totalTime) - 1 / 2)) + m_start.z;
	m_pos.x = m_look.x;
	m_pos.y = m_look.y;
	m_pos.z = m_look.z - 3.0f;
	m_time += 1.0f / 60.0f;
}

void CameraEvent::SetEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time)
{
	m_start = startPos;
	m_end = endPos;
	m_time = 0.0f;
	m_totalTime = time;
}

bool CameraEvent::IsEvent()
{
	return m_time < m_totalTime;
}
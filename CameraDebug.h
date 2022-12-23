#pragma once
#include "Input.h"
#include "CameraBase.h"
class CameraDebug : public CameraBase
{
public:
	CameraDebug();
	~CameraDebug();
	void Update();
private:
	float m_radY;
	float m_radXZ;
	float m_radius;
};
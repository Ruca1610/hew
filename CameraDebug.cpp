#include "CameraDebug.h"

CameraDebug::CameraDebug()
	:m_radY(0.0f)
	, m_radXZ(0.0f)
	, m_radius(5.0f)
{

}

CameraDebug::~CameraDebug()
{

}

 void CameraDebug::Update()
 {
 //// ←、↑、→、↓、Shift、Ctrlでカメラの注視点を動かす
 //if (IsKeyPress(VK_UP)) m_look.z += 0.01f;
 //if (IsKeyPress(VK_DOWN)) m_look.z -= 0.01f;
 //if (IsKeyPress(VK_RIGHT)) m_look.x += 0.01f;
 //if (IsKeyPress(VK_LEFT)) m_look.x -= 0.01f;
 //if (IsKeyPress(VK_SHIFT)) m_look.y += 0.01f;
 //if (IsKeyPress(VK_CONTROL)) m_look.y -= 0.01f;
 // W,A,S,D,Q,Eでカメラを回転させる
 if (IsKeyPress('W'))	m_radY += 0.1f;
 if (IsKeyPress('A'))	m_radXZ -= 0.1f;
 if (IsKeyPress('S'))	m_radY -= 0.1f;
 if (IsKeyPress('D'))	m_radXZ += 0.1f;
 // Q,Eでカメラと注視点の距離を変える
 if (IsKeyPress('Q'))	m_radius += 0.1f;
 if (IsKeyPress('E'))	m_radius -= 0.1f;

 // 角度と距離、注視点からカメラの位置を計算
 m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius + m_look.x;
 m_pos.y = sin(m_radY) * m_radius + m_look.y;
 m_pos.z = cos(m_radY) * cos(m_radXZ) * m_radius + m_look.z;
}
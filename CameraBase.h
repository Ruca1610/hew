#pragma once
#include <DirectXMath.h>
class CameraBase 
{
public:
	CameraBase();
	virtual ~CameraBase();
	virtual void Update() = 0;

	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetLook();

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
protected:
	DirectX::XMFLOAT3 m_pos, m_look, m_up;	// �r���[�s��̐ݒ�ɕK�v�ȕϐ�
	float m_fovy, m_aspect, m_near, m_far;	// �v���W�F�N�V�����s��̐ݒ�ɕK�v�ȕϐ�
};
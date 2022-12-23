#pragma once
#include "CameraBase.h"
#include "Model.h"
#include "ConstantBuffer.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void Update();
	void Draw();

	void SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos();
	void Collision(DirectX::XMFLOAT3 pos);
	void Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal, DirectX::XMFLOAT3 out);
	DirectX::XMFLOAT3 GetVector();
	void SetVector(DirectX::XMVECTOR);
	void SetPos(float x, float y, float z);
	float GetCurrentBullet();
	void Reflect(DirectX::XMVECTOR vector);
private:
	DirectX::XMFLOAT3 m_pos;
	Model* m_pModel;
	VertexShader* m_pVS;
	ConstantBuffer* m_pWVP;
	CameraBase* m_pCamera;

	float m_speed;

	DirectX::XMVECTOR vFront;
	DirectX::XMVECTOR vMove;


	DirectX::XMFLOAT3 PlayerPos;

	float m_CurrentCntBullet;

};
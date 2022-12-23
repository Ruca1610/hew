#include "Bullet.h"
#include "Input.h"
#include <DirectXMath.h>

Bullet::Bullet()
	: m_pos(5.0f, 2.0f, 1.0f)
	, PlayerPos(m_pos)
	, m_speed(0.0f)
	, m_pCamera(NULL)
	, m_CurrentCntBullet(0.0f)
{
	m_pModel = new Model();
	m_pModel->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);

	m_pVS = new VertexShader;
	if (FAILED(m_pVS->Load("Assets/Shader/ModelVS.cso")))
		MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);
	m_pModel->SetVertexShader(m_pVS);

	m_pWVP = new ConstantBuffer();
	m_pWVP->Create(sizeof(DirectX::XMFLOAT4X4) * 3);
	vMove = DirectX::XMVectorSet(0.1, 0, 0.1, 0);
}

Bullet::~Bullet()
{
	delete m_pWVP;
	delete m_pVS;
	delete m_pModel;
}

void Bullet::Update()
{
	m_CurrentCntBullet++;

	vMove = DirectX::XMVectorZero();
	vMove = DirectX::XMVectorAdd(vMove, vFront);
	vMove = DirectX::XMVectorScale(vMove, 0.1f);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, vMove);

	m_pos.x += move.x;
	m_pos.y += move.y;
	m_pos.z += move.z;
}

void Bullet::Draw()
{

	if (!m_pCamera)
	{
		return;
	}
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();
	m_pWVP->Write(mat);
	m_pWVP->BindVS(0);
	m_pModel->Draw();
}

void Bullet::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Bullet::GetPos()
{
	DirectX::XMFLOAT3 pos = m_pos;
	pos.y += 0.1f;
	return pos;
}

void Bullet::Collision(DirectX::XMFLOAT3 pos)
{
	PlayerPos = pos;
	DirectX::XMFLOAT3 BulletPos = m_pos;
	PlayerPos.y = 0.0f;
	BulletPos.y = 0.0f;
	DirectX::XMVECTOR vPlayerPos = DirectX::XMLoadFloat3(&PlayerPos);
	DirectX::XMVECTOR vBulletPos = DirectX::XMLoadFloat3(&BulletPos);

	vFront = DirectX::XMVectorSubtract(vBulletPos, vPlayerPos);
	vFront = DirectX::XMVector3Normalize(vFront);
	m_speed = 0.25f;
}

void Bullet::Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal, DirectX::XMFLOAT3 out)
{
	if (planenormal.y > 0.5f)
	{
		m_pos.y = hitpoint.y;
	}
	else
	{
		m_pos.y = hitpoint.y - 0.1f;
		m_pos.x = hitpoint.x;
		m_pos.z = hitpoint.z;
	}

	vFront = DirectX::XMLoadFloat3(&out);
}

DirectX::XMFLOAT3 Bullet::GetVector()
{
	DirectX::XMFLOAT3 Vector;
	DirectX::XMStoreFloat3(&Vector, vFront);
	return Vector;
}

void Bullet::SetVector(DirectX::XMVECTOR vector)
{
	vFront = vector;
}

float Bullet::GetCurrentBullet()
{
	return m_CurrentCntBullet;
}

void Bullet::Reflect(DirectX::XMVECTOR vector)
{
	vFront = vector;
}


void Bullet::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}
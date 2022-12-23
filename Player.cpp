#include "Player.h"
#include "Input.h"
#include <DirectXMath.h>
#include <DirectXTex/Texture.h>
#include "Sprite.h"

Player::Player(static Model* ModelList)
	:m_pos(1.0f, 0.5f, 1.0f)
	, m_gravity(-0.04f)
	, m_acc(0.0f)
	, m_CanJump(true)
	, m_pCamera(NULL)
{
	//m_pModel = new Model();
	//m_pModel->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);

	m_pModel = ModelList;
	m_pVS = new VertexShader;
	if (FAILED(m_pVS->Load("Assets/Shader/ModelVS.cso")))
		MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);
	m_pModel->SetVertexShader(m_pVS);

	// 定数バッファ作成
	m_pWVP = new ConstantBuffer();
	m_pWVP->Create(sizeof(DirectX::XMFLOAT4X4) * 3);

	// 影の初期化
	LoadTextureFromFile("2D/shadow.png", &m_pShadowTex);
	m_shadowPos = m_pos;

	// バーの初期化
	LoadTextureFromFile("2D/Glass19.png", &m_pBarTex);

	// 軌跡の初期化
	m_pPolyline = new GeometoryPolyline(15);
}

Player::~Player()
{
	delete m_pPolyline;
	m_pBarTex->Release();
	m_pShadowTex->Release();
	delete m_pWVP;
	delete m_pVS;
	//	delete m_pModel;//	ModelListでdelete
}

void Player::Update()
{
	DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();
	DirectX::XMFLOAT3 camLook = m_pCamera->GetLook();
	camPos.y = 0.0f;
	camLook.y = 0.0f;
	DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
	DirectX::XMVECTOR vCamLook = DirectX::XMLoadFloat3(&camLook);

	DirectX::XMVECTOR vFront;
	vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
	vFront = DirectX::XMVector3Normalize(vFront);

	DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationY((90 * 3.14f) / 180.0f);
	DirectX::XMVECTOR vSide = DirectX::XMVector3TransformCoord(vFront, matRotSide);

	DirectX::XMVECTOR vMove = DirectX::XMVectorZero();
	if (IsKeyPress('W'))	vMove = DirectX::XMVectorAdd(vMove, vFront);
	if (IsKeyPress('A'))	vMove = DirectX::XMVectorSubtract(vMove, vSide);
	if (IsKeyPress('S'))	vMove = DirectX::XMVectorSubtract(vMove, vFront);
	if (IsKeyPress('D'))	vMove = DirectX::XMVectorAdd(vMove, vSide);
	vMove = DirectX::XMVectorScale(vMove, 0.1f);

	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, vMove);

	if (m_CanJump && IsKeyTrigger(VK_SPACE))
	{
		m_CanJump = false;
		m_acc = 0.4f;
	}

	m_acc += m_gravity;
	if (m_acc < -0.25f)
	{
		m_acc = -0.25f;
	}
	move.y += m_acc;//

	m_pos.x += move.x;
	m_pos.y += move.y;
	m_pos.z += move.z;

	// 影の更新
	// 本当は当たり判定を使って
	// 地面の箇所に配置できたほうがよい
	m_shadowPos = m_pos;
	//m_shadowPos.y = 0.0f;
}

void Player::Draw()
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

	// 影の描画
	//DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation
	(
		m_shadowPos.x, m_shadowPos.y, m_shadowPos.z
	);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationX
	(
		DirectX::XMConvertToRadians(90.0f)
	);
	DirectX::XMStoreFloat4x4(&mat[0], // world行列
		DirectX::XMMatrixTranspose(R * T));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);

	Sprite::SetSize(DirectX::XMFLOAT2(0.3f, 0.3f));
	Sprite::SetTexture(m_pShadowTex);

	Sprite::Draw();

	// バーの表示
	// カメラの回転を打ち消す行列(逆行列)を求める
	DirectX::XMFLOAT4X4 fInv;
	DirectX::XMMATRIX matInv;
	fInv = m_pCamera->GetViewMatrix();
	matInv = DirectX::XMLoadFloat4x4(&fInv);
	// 転置行列から一度元に戻す
	matInv = DirectX::XMMatrixTranspose(matInv);
	// カメラの回転だけを打ち消したいので、
	// カメラの移動成分は0(移動しないよう)にする
	// ただし、行列の中身を直接変える場合はFLOAT4x4で
	// ないといけないので、MATRIXからFLOAT4x4に変換する
	DirectX::XMStoreFloat4x4(&fInv, matInv);
	fInv._41 = fInv._42 = fInv._43 = 0.0f;
	// 逆行列への返還はMATRIX型で行うのでまた変換...
	matInv = DirectX::XMLoadFloat4x4(&fInv);
	matInv = DirectX::XMMatrixInverse(nullptr, matInv);

	// バーを配置する行列(ワールド)を求める
	T = DirectX::XMMatrixTranslation(
		m_pos.x, m_pos.y + 1.7f, m_pos.z
	);
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(matInv * T));
	Sprite::SetWorld(mat[0]);
	// ViewとProjectionは影を表示する時に設定したので今回は設定しない
	Sprite::SetSize(DirectX::XMFLOAT2(0.6f, 0.04f));
	Sprite::SetTexture(m_pBarTex);
	EnableDepth(false);
	Sprite::Draw();
	EnableDepth(true);

	// 軌跡の追加
	GeometoryPolyline::Point point = {};
	point.pos = m_pos;
	point.width = 0.7f;
	// 設置した位置からカメラに向かうベクトルがポリラインの法線となる
	DirectX::XMVECTOR vCamPos =
		DirectX::XMLoadFloat3(&m_pCamera->GetPos());
	DirectX::XMVECTOR vPos =
		DirectX::XMLoadFloat3(&m_pos);
	DirectX::XMStoreFloat3(&point.normal,
		DirectX::XMVectorSubtract(vCamPos, vPos));
	m_pPolyline->PushPoint(point);
	// 軌跡の表示
	m_pPolyline->SetView(m_pCamera->GetViewMatrix());
	m_pPolyline->SetProjection(m_pCamera->GetProjectionMatrix());
	m_pPolyline->Draw();
}

void Player::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Player::GetPos()
{
	DirectX::XMFLOAT3 pos = m_pos;
	pos.y += 0.2f;
	return pos;
}

void Player::Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal)
{
	if (planenormal.y > 0.5f)
	{
		m_pos.y = hitpoint.y;
	}
	else
	{
		m_pos.y = hitpoint.y - 0.2f;
		m_pos.x = hitpoint.x;
		m_pos.z = hitpoint.z;
	}

	m_acc = 0.0f;
	m_CanJump = true;
}
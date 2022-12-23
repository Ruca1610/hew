#include "UI.h"
#include "Sprite.h"
#include "DirectXTex/Texture.h"

UI::UI()
{
	LoadTextureFromFile("2D/Glass19.png", &m_pBar);
}

UI::~UI()
{
	m_pBar->Release();
}

void UI::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];
	// 2Dで配置する位置を決める
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(1110.0f, 690.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(T));
	// 2Dではカメラが不要なので、変化しない行列を設定する
	// 変化しない行列...中身が全部0だと、掛け算すると全部0になる
	// 拡大・縮小の部分に1が固定されている行列が必要
	// 変化しない行列を"単位行列"という
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	// カメラがまっすぐ見ている(平行投影)行列を設定
	DirectX::XMStoreFloat4x4(&mat[2],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicOffCenterLH(
				0.0f,		// 画面左端の座標
				1280.0f,	// 画面右端の座標
				720.0f,		// 画面下端の座標
				0.0f,		// 画面上端の座標
				-1.0f,		// z座標で写せる範囲の手前
				1.0f		// z座標で写せる範囲の億奥
			)));

	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -20.0f));
	Sprite::SetTexture(m_pBar);
	Sprite::Draw();
}
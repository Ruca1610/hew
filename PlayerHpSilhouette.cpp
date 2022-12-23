////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		PlayerHpSilhouette.cpp
//		
//		更新日時	更新した人	更新内容
//		----------------------------------
//		2022/12/14	森本		PlayerHpPlayerHpSilhouette.h/cpp作成
//		
//		
//		
//		内容
//		・プレイヤーのHPバー関連のプログラムです
//
//
//		注意点
//		・このcppの関数はGameUI.cppで基本的に呼びだしてください
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "PlayerHpSilhouette.h"
#include "Gauge.h"
#include "DirectXTex/Texture.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	PlayerHpSilhuette.cpp:コンストラクタ
//
//	クラス名：CPlayerHpSilhouette
//
//	変数名			型名						概要
//	m_fPlayerHpPos	Float3						HPバーの背景の位置
//	m_fMaxHp		float						HPバーの背景の長さ
//	m_pBar			ID3D11ShaderResourceView	HPバーの背景のテクスチャ
//
//	注意点
//	・HPバーに貼るテクスチャは上下左右がきっちり揃っている画像でないとダメージを受けた際にズレていってしまう
//	・PlayerHpと同じ長さ、位置にしてテクスチャは違う色のものを使用してください
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPlayerHpSilhouette::CPlayerHpSilhouette()
{
	m_fPlayerHpPos = DirectX::XMFLOAT3(70.0f, 30.0f, 0.0f);
	m_fMaxHp = 200.0f;
	LoadTextureFromFile("Assets/HpBarSilhouette.png", &m_pBar);
}

CPlayerHpSilhouette::~CPlayerHpSilhouette()
{
	m_pBar->Release();
}

void CPlayerHpSilhouette::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];;
	DirectX::XMMATRIX T =
		DirectX::XMMatrixTranslation(m_fPlayerHpPos.x, m_fPlayerHpPos.y, m_fPlayerHpPos.z);
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(T));
	DirectX::XMStoreFloat4x4(&mat[1],
		DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mat[2],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixOrthographicOffCenterLH(
				0.0f,		// 画面左端の座標
				1280.0f,	// 画面右端の座標
				720.0f,		// 画面下端の座標
				0.0f,		// 画面上端の座標
				-1.0f,		// Ｚ座標で写せる範囲の手前
				1.0f		// Ｚ座標で写せる範囲の奥
			)));

	CGauge::SetWorld(mat[0]);
	CGauge::SetView(mat[1]);
	CGauge::SetProjection(mat[2]);
	CGauge::SetSize(DirectX::XMFLOAT2(m_fMaxHp, -20.0f));
	CGauge::SetTexture(m_pBar);
	CGauge::Draw();
}

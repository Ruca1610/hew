////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		PlayerHp.cpp
//		
//		更新日時	更新した人	更新内容
//		----------------------------------
//		2022/12/13	森本		PlayerHp.h/cpp作成
//		2022/12/14　森本		コメントの追加
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

#include "PlayerHp.h"
#include "Gauge.h"
#include "DirectXTex/Texture.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	PlayerHp.cpp:コンストラクタ
//
//	クラス名：CPlayerHp
//
//	変数名			型名						概要
//	m_fPlayerHpPos	Float3						HPバーの位置
//	m_fHp			float						HPバーの長さ
//	m_fDamage		float						ダメージ量（これによってHPバーの減り方が変わる）
//	m_nHeal			int							回復するまでのフレームのカウント
//	m_nFrame		float						フレームのカウント
//	m_bInv			bool						無敵時間の処理用
//	m_bInvTime		int							無敵時間
//	m_pBar			ID3D11ShaderResourceView	HPバーのテクスチャ
//	※HPバーに貼るテクスチャは上下左右がきっちり揃っている画像でないとダメージを受けた際にズレていってしまう
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPlayerHp::CPlayerHp()
{
	m_fPlayerHpPos = DirectX::XMFLOAT3(70.0f, 30.0f, 0.0f);
	m_fHp = 200.0f;
	m_fDamage = 20;
	m_nHeal = 0;
	m_nFrame = 0;
	m_bInv = false;
	m_nInvTime = 120;
	LoadTextureFromFile("Assets/HpBar.png", &m_pBar);
}

CPlayerHp::~CPlayerHp()
{
	m_pBar->Release();
}

void CPlayerHp::Draw()
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
	CGauge::SetSize(DirectX::XMFLOAT2(m_fHp, -20.0f));
	CGauge::SetTexture(m_pBar);
	CGauge::Draw();
}

void CPlayerHp::Update()
{
	// 回復関連（不完全）
	m_nHeal++;
	if (m_fHp < 200)
	{
		if (m_nHeal > 300)
		{
			m_fHp += 10;
			CGauge::Draw();
			m_nHeal = 0;
		}
	}
	else
	{
		m_nHeal = 0;
	}


	// 無敵時間関連
	if (m_bInv == true)
		m_nFrame++;

	if (m_nFrame > 120)
	{
		m_bInv = false;
		m_nFrame = 0;
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//
//	PlayerHp.cpp：PlayerDamage関数
//
//	class：CPlayerHp
//
//	関数名			引数						
//	SetSize			Float2（ HPバーの横の長さ ,  HPバーの縦の長さ ）
//	
//	・プレイヤーがダメージを受ける関数になります
//	・HPが0になった処理はまだ書いていません(作ったらこのコメントを消してください)
//	
//	使用場所
//	・GameUI.cppのDamage関数内
//	
//	
//////////////////////////////////////////////////////////////////////////////////////
void CPlayerHp::PlayerDamage()
{
	if (!m_bInv == true)
	{
		m_fHp -= m_fDamage;
		CGauge::SetSize(DirectX::XMFLOAT2(m_fHp, -20.0f));
		CGauge::Draw();
		m_bInv = true;
	}
}


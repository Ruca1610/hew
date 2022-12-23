#pragma once
#include "CameraBase.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "Player.h"

class Enemy
{
public:
	Enemy(static Model* ModelList);
	~Enemy();
	void Update();
	void Draw();
	void Effect();
	void Touch();

	void SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos();
	void Collision(DirectX::XMFLOAT3 pos);
	void Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal, DirectX::XMFLOAT3 out);
	DirectX::XMFLOAT3 GetVector();
	void SetPos(float x, float y, float z);
	//-----追加関数----
	//プレイヤーに触られていたらbTouchをtrueにする	
	void TochtEnemy();
	//プレイヤーに触られているかどうか
	bool CheckTouch();
	//エネミーが死んでいるかどうか
	bool CheckEnemy();
	//フレーム数の加算
	float GetCurrent();

private:

	DirectX::XMFLOAT3 m_pos;
	Model* m_pModel;
	VertexShader* m_pVS;
	ConstantBuffer* m_pWVP;
	CameraBase* m_pCamera;
	Player* m_pPlayer;

	// 影の情報
	DirectX::XMFLOAT3 m_shadowPos;
	ID3D11ShaderResourceView* m_pShadowTex;

	//エフェクト
	ID3D11ShaderResourceView* m_pEffectTex;
	float m_uvX;	//スプライトシートのX座標
	float m_uvY;	//スプライトシートのY座標
	bool isTouch;
	int m_frame;

	float m_CurrentCnt = 0;

	float m_speed;
	DirectX::XMVECTOR vFront;
	DirectX::XMVECTOR vMove;
	//-----追加関数----
	//エネミーが攻撃されたかの判定
	bool m_bTouch;
	//エネミーが生きているか死んでいるか判定
	bool m_bEnemy;
	//モデル読み込みされているか
	static bool LoadModel;
	//---------------------
	DirectX::XMFLOAT3 PlayerPos;
};
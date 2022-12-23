#ifndef __GAME_3D_H__
#define __GAME_3D_H__

#include "Model.h"
#include "Stage.h"
#include "Geometory.h"
#include "ConstantBuffer.h"
#include "CameraBase.h"
#include "CameraDebug.h"
#include "Player.h"
#include "CameraPlayer.h"
#include "CameraEvent.h"
#include "Collision.h"
#include "Enemy.h"
#include "BOSS.h"
#include "Bullet.h"
#include "BlendState.h"
#include "GameUI.h"
#include "UI.h"
#include "ModelList.h"


class Game3D
{
public:
	Game3D();
	~Game3D();

	void Update();
	void Draw();

	enum CameraKind
	{
		CAM_PLAYER,
		CAM_DEBUG,
		CAM_EVENT,	// イベント用
		MAX_CAMERA	// カメラ最大数
	};

	DirectX::XMFLOAT3 calcReflectVector(const DirectX::XMFLOAT3& front, const DirectX::XMFLOAT3& normal);
	//エネミーを再生成する
	void RespawnEnemy();
	void AttackEnemy();
	void CollisionEnemy();

private:
	CameraBase* m_pCamera[MAX_CAMERA];	// もともとは一台分しかなかったが、複数台用意
	CameraKind m_mainCamera;			// 現在、表示に利用しているカメラの番号
	BlendState* m_pBlend;
	UI* m_pUI;
	VertexShader* m_pModelVS;
	ConstantBuffer* m_pWVP;
	Model*	m_pModel;
	Stage* m_pStage;
	Player* m_pPlayer;
	ModelList* m_pModelList;
	GameUI* m_pGameUI;
	BOSS* m_pBOSS;
	Bullet* m_pBullet;


	DirectX::XMFLOAT3 fEnemyPos;
	float m_MaxCurrent = 0;
	bool HitPlayer;	// 弾がプレイヤーに当たったかどうか

	int m_frame = 0;
	//何ウェーブなのかを管理
	int m_nCnt = 0;
};

#endif // __GAME_3D_H__
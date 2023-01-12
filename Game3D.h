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
#include "Sound.h"


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
		CAM_EVENT,	// �C�x���g�p
		MAX_CAMERA	// �J�����ő吔
	};

	DirectX::XMFLOAT3 calcReflectVector(const DirectX::XMFLOAT3& front, const DirectX::XMFLOAT3& normal);
	//�G�l�~�[���Đ�������
	void RespawnEnemy();
	void AttackEnemy();
	void CollisionEnemy();

	float Volume();

private:
	CameraBase* m_pCamera[MAX_CAMERA];	// ���Ƃ��Ƃ͈�䕪�����Ȃ��������A������p��
	CameraKind m_mainCamera;			// ���݁A�\���ɗ��p���Ă���J�����̔ԍ�
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
	bool HitPlayer;	// �e���v���C���[�ɓ����������ǂ���

	int m_frame = 0;
	//���E�F�[�u�Ȃ̂����Ǘ�
	int m_nCnt = 0;

	//���ʂ̒��ߗp
	float m_volume;
};

#endif // __GAME_3D_H__
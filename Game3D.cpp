#include "Game3D.h"
#include<time.h>
const int MAX_ENEMY = 10;
const int LIMIT_POS_RIGHT_X = 15;
const int LIMIT_POS_LEFT_X = 15;
const int LIMIT_POS_FRONT_Z = 15;
const int LIMIT_POS_BACK_Z = 15;

Enemy* m_pEnemy[MAX_ENEMY];
bool Hit = false;;

Game3D::Game3D()
	:HitPlayer(false)
	,m_MaxCurrent(18000)
{
	//rand()を完全ランダムにてくれる処理
	srand((unsigned int)time(nullptr));

	m_pModelList = new ModelList;

	m_pPlayer = new Player(m_pModelList->GetModelList(MODEL_PLAYER));
	m_pBOSS = new BOSS();
	m_pBullet = new Bullet();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		m_pEnemy[i] = new Enemy(m_pModelList->GetModelList(MODEL_PLAYER));
		m_pEnemy[i]->SetPos((rand() % LIMIT_POS_RIGHT_X) - (rand() % LIMIT_POS_LEFT_X),
			0.5f,
			(rand() % LIMIT_POS_FRONT_Z) - (rand() % LIMIT_POS_BACK_Z));
	}

	m_pStage = new Stage();

	m_mainCamera = CAM_EVENT;
	m_pCamera[CAM_PLAYER] = new CameraPlayer(m_pPlayer);
	m_pCamera[CAM_DEBUG] = new CameraDebug();
	CameraEvent* pEvent = new CameraEvent();
	pEvent->SetEvent(DirectX::XMFLOAT3(5.0f, 1.7f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 0.1f);
	m_pCamera[CAM_EVENT] = pEvent;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		m_pEnemy[i]->SetCamera(m_pCamera[CAM_EVENT]);
	}
	m_pBOSS->SetCamera(m_pCamera[CAM_EVENT]);
	m_pBullet->SetCamera(m_pCamera[CAM_EVENT]);
	m_pPlayer->SetCamera(m_pCamera[CAM_EVENT]);

	// ブレンドステート作成
	D3D11_RENDER_TARGET_BLEND_DESC blend = {};
	blend.BlendEnable = true;
	blend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blend.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.DestBlendAlpha = D3D11_BLEND_ZERO;
	blend.BlendOp = D3D11_BLEND_OP_ADD;
	blend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	m_pBlend = new BlendState();
	m_pBlend->Create(blend);
	m_pBlend->Bind();

	// UI作成
	m_pGameUI = new GameUI();
	m_pUI = new UI();

	//// プレイヤーを追尾する弾の情報
	//DirectX::XMFLOAT3 fEnemyPos = m_pBOSS->GetPos();
	//DirectX::XMVECTOR vEnemyPos = DirectX::XMLoadFloat3(&fEnemyPos);
	//DirectX::XMFLOAT3 fPlayerPos = m_pPlayer->GetPos();
	//DirectX::XMVECTOR vPlayerPos = DirectX::XMLoadFloat3(&fPlayerPos);
	//DirectX::XMVECTOR vSpeed = DirectX::XMVectorScale(DirectX::XMVectorSubtract(vPlayerPos, vEnemyPos), 0.002f);	// 弾の速さ
	//DirectX::XMFLOAT3 debg;
	//DirectX::XMStoreFloat3(&debg, vSpeed);
	//m_pBullet->SetVector(vSpeed);
}
Game3D::~Game3D()
{
	delete m_pUI;
	delete m_pGameUI;
	delete m_pBlend;
	delete m_pStage;
	delete m_pBullet;
	delete m_pBOSS;
	delete m_pPlayer;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		delete m_pEnemy[i];
	}
	for (int i = 0; i < MAX_CAMERA; i++)
	{
		delete m_pCamera[i];
	}
	delete m_pModelList;

}
void Game3D::Update()
{

	CameraKind camera = m_mainCamera;

	m_pCamera[m_mainCamera]->Update();


	if (m_mainCamera == CAM_EVENT)
	{
		CameraEvent* pEvent = reinterpret_cast<CameraEvent*>(m_pCamera[m_mainCamera]);

		if (!pEvent->IsEvent())
		{
			camera = CAM_PLAYER;
		}
	}

	if (m_mainCamera == CAM_PLAYER)
	{
		m_pPlayer->Update();
	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{

		if (m_pEnemy[i] != nullptr)
		{
			m_pEnemy[i]->Update();

			//speedが0になったら
			//エネミーがプレイヤーに飛ばされていたら削除
			if (m_pEnemy[i]->CheckEnemy() && m_pEnemy[i]->CheckTouch())
			{
				delete m_pEnemy[i];
				m_pEnemy[i] = nullptr;
				
			}
		}
		//エネミーが全員死んだら次ウェーブに行く
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			if (m_pEnemy[i] != nullptr)
			{
				break;
			}
			//最終ループだったら
			if (i == MAX_ENEMY - 1)
			{
				RespawnEnemy();
				m_nCnt++;
			}
		}
	}

	if (!Hit)
	{
		DirectX::XMFLOAT3 BulletPos = m_pBullet->GetPos();
		DirectX::XMVECTOR vBulletPos = DirectX::XMLoadFloat3(&BulletPos);
		DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();
		DirectX::XMVECTOR vPlayerPos = DirectX::XMLoadFloat3(&PlayerPos);
		DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vPlayerPos, vBulletPos);

		//正規化
		vFront = DirectX::XMVector3Normalize(vFront);

		m_pBullet->SetVector(vFront);
	}

	if (IsKeyTrigger(VK_RETURN))
	{
		Hit = true;
		if (m_pBOSS)
		{
			DirectX::XMFLOAT3 BOSSPos = m_pBOSS->GetPos();
			DirectX::XMVECTOR vBOSSPos = DirectX::XMLoadFloat3(&BOSSPos);
			DirectX::XMFLOAT3 BulletPos = m_pBullet->GetPos();
			DirectX::XMVECTOR vBulletPos = DirectX::XMLoadFloat3(&BulletPos);
			DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(vBOSSPos, vBulletPos);

			//正規化
			vFront = DirectX::XMVector3Normalize(vFront);

			m_pBullet->Reflect(vFront);
		}
	}

	// 弾の更新
	if(m_pBullet) m_pBullet->Update();


	if (IsKeyPress('C'))	// カメラの切り替えはあくまでデバッグなので簡単に切り替えられないようにする
	{
		if (IsKeyTrigger('1'))
		{
			camera = CAM_DEBUG;
		}
		if (IsKeyTrigger('2'))
		{
			camera = CAM_PLAYER;
		}
		if (IsKeyTrigger('3'))
		{
			CameraEvent* pEvent = reinterpret_cast<CameraEvent*>(m_pCamera[CAM_EVENT]);
			pEvent->SetEvent(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f), m_pPlayer->GetPos(), 5.0f);
			camera = CAM_EVENT;
		}
	}

	if (m_mainCamera != camera)
	{
		m_mainCamera = camera;
		m_pPlayer->SetCamera(m_pCamera[m_mainCamera]);
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			m_pEnemy[i]->SetCamera(m_pCamera[m_mainCamera]);
		}
		m_pBOSS->SetCamera(m_pCamera[m_mainCamera]);
		m_pBullet->SetCamera(m_pCamera[m_mainCamera]);
	}
	
	// 弾とプレイヤーの当たり判定
	if (m_pBullet != nullptr)
	{
		if ((m_pPlayer->GetPos().x - m_pBullet->GetPos().x) * (m_pPlayer->GetPos().x - m_pBullet->GetPos().x) +
			(m_pPlayer->GetPos().y - m_pBullet->GetPos().y) * (m_pPlayer->GetPos().y - m_pBullet->GetPos().y) +
			(m_pPlayer->GetPos().z - m_pBullet->GetPos().z) * (m_pPlayer->GetPos().z - m_pBullet->GetPos().z)
			<= (0.5f + 0.5f) * (0.5f + 0.5f))
		{
			//m_pBullet->Reflect();
			HitPlayer = true;
		}
	}

	// ボスと弾の当たり判定
	if (HitPlayer)
	{
		if (m_pBOSS)
		{
			if ((m_pBOSS->GetPos().x - m_pBullet->GetPos().x) * (m_pBOSS->GetPos().x - m_pBullet->GetPos().x) +
				(m_pBOSS->GetPos().y - m_pBullet->GetPos().y) * (m_pBOSS->GetPos().y - m_pBullet->GetPos().y) +
				(m_pBOSS->GetPos().z - m_pBullet->GetPos().z) * (m_pBOSS->GetPos().z - m_pBullet->GetPos().z)
				<= (0.5f + 0.5f) * (0.5f + 0.5f))
			{
				delete m_pBOSS;
				m_pBOSS = nullptr;
				delete m_pBullet;
				m_pBullet = nullptr;
			}
		}
	}

	// 敵とプレイヤーの攻撃判定
	// x1     x2
	AttackEnemy();
	// プレイヤーがダメージを受ける時の当たり判定(敵とプレイヤーの当たり判定)
	CollisionEnemy();

	// HPが減るUI
	m_pGameUI->Update();

	// ステージとの当たり判定
	for (int i = 0; i < m_pStage->GetBlockNum(); i++)
	{
		Stage::Info block = m_pStage->GetBlockInfo(i);
		Stage::Vertices vtx = m_pStage->GetBlockVertices(i);

		DirectX::XMFLOAT3 planeN = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

		// ブロックの角度から回転行列を計算
		DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&planeN);
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(block.rot.x);
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(block.rot.y);
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(block.rot.z);
		vPlaneN = DirectX::XMVector3TransformCoord(vPlaneN, Rx * Ry * Rz);
		vPlaneN = DirectX::XMVector3Normalize(vPlaneN);
		DirectX::XMStoreFloat3(&planeN, vPlaneN);

		// 当たり判定
		Collision::Plane plane = { planeN,vtx.pos[0] };
		Collision::Ray ray = { m_pPlayer->GetPos(),DirectX::XMFLOAT3(0.0f,-1.0f,0.0f),0.5f };
		Collision::Result resultPlayer = Collision::CheckRayPlane(ray, plane);
		Collision::Result resultEnemy = Collision::CheckRayPlane(ray, plane);
		Collision::Triangle triangle[2];
		if (resultPlayer.hit)
		{
			triangle[0] = { vtx.pos[0],vtx.pos[1],vtx.pos[2] };
			triangle[1] = { vtx.pos[1],vtx.pos[3],vtx.pos[2] };

			DirectX::XMFLOAT3 point = resultPlayer.point;

			for (int j = 0; j < 2; ++j) // 三角形の数だけ繰り返し処理
			{
				resultPlayer = Collision::CheckPointTriangle(point, triangle[j]);
				if (resultPlayer.hit)
				{
					m_pPlayer->Landing(resultPlayer.point, plane.normal);
					break;
				}
			}
		}
		if (resultEnemy.hit)
		{
			triangle[0] = { vtx.pos[0],vtx.pos[1],vtx.pos[2] };
			triangle[1] = { vtx.pos[1],vtx.pos[3],vtx.pos[2] };

			DirectX::XMFLOAT3 point = resultEnemy.point;

			for (int j = 0; j < 2; ++j) // 三角形の数だけ繰り返し処理
			{
				resultEnemy = Collision::CheckPointTriangle(point, triangle[j]);
				if (resultEnemy.hit)
				{
					DirectX::XMFLOAT3 Zero = { 0.0f,0.0f,0.0f };
					//m_pEnemy->Landing(resultEnemy.point, plane.normal,Zero);
					break;
				}
			}
		}

		// 正面
		planeN = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

		vPlaneN = DirectX::XMLoadFloat3(&planeN);
		Rx = DirectX::XMMatrixRotationX(block.rot.x);
		Ry = DirectX::XMMatrixRotationY(block.rot.y);
		Rz = DirectX::XMMatrixRotationZ(block.rot.z);
		vPlaneN = DirectX::XMVector3TransformCoord(vPlaneN, Rx * Ry * Rz);
		vPlaneN = DirectX::XMVector3Normalize(vPlaneN);
		DirectX::XMStoreFloat3(&planeN, vPlaneN);

		plane = { planeN, vtx.pos[2] };

		triangle[0] = { vtx.pos[2],vtx.pos[3],vtx.pos[6] };
		triangle[1] = { vtx.pos[3],vtx.pos[7],vtx.pos[6] };


		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//プレイヤー
			resultPlayer = Collision::CheckCirclePlane(m_pPlayer->GetPos(), 0.5f, plane, triangle);
			if (resultPlayer.hit)
			{
				m_pPlayer->Landing(resultPlayer.point, plane.normal);
			}

			//エネミー
			if (m_pEnemy[i] != nullptr)
			{
				resultEnemy = Collision::CheckCirclePlane(m_pEnemy[i]->GetPos(), 1.5f, plane, triangle);
				if (resultEnemy.hit)
				{
					DirectX::XMFLOAT3 out = calcReflectVector(m_pEnemy[i]->GetVector(), planeN);
					m_pEnemy[i]->Landing(resultEnemy.point, plane.normal, out);
				}
			}

			// 左面
			planeN = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);

			vPlaneN = DirectX::XMLoadFloat3(&planeN);
			Rx = DirectX::XMMatrixRotationX(block.rot.x);
			Ry = DirectX::XMMatrixRotationY(block.rot.y);
			Rz = DirectX::XMMatrixRotationZ(block.rot.z);
			vPlaneN = DirectX::XMVector3TransformCoord(vPlaneN, Rx * Ry * Rz);
			vPlaneN = DirectX::XMVector3Normalize(vPlaneN);
			DirectX::XMStoreFloat3(&planeN, vPlaneN);

			plane = { planeN, vtx.pos[0] };

			triangle[0] = { vtx.pos[0],vtx.pos[2],vtx.pos[4] };
			triangle[1] = { vtx.pos[2],vtx.pos[6],vtx.pos[4] };


			resultPlayer = Collision::CheckCirclePlane(m_pPlayer->GetPos(), 0.5f, plane, triangle);
			if (resultPlayer.hit)
			{
				m_pPlayer->Landing(resultPlayer.point, plane.normal);
			}

			if (m_pEnemy[i] != nullptr)
			{

				resultEnemy = Collision::CheckCirclePlane(m_pEnemy[i]->GetPos(), 1.5f, plane, triangle);
				if (resultEnemy.hit)
				{
					DirectX::XMFLOAT3 out = calcReflectVector(m_pEnemy[i]->GetVector(), planeN);
					m_pEnemy[i]->Landing(resultEnemy.point, plane.normal, out);
				}
			}
			// 右面
			planeN = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

			vPlaneN = DirectX::XMLoadFloat3(&planeN);
			Rx = DirectX::XMMatrixRotationX(block.rot.x);
			Ry = DirectX::XMMatrixRotationY(block.rot.y);
			Rz = DirectX::XMMatrixRotationZ(block.rot.z);
			vPlaneN = DirectX::XMVector3TransformCoord(vPlaneN, Rx * Ry * Rz);
			vPlaneN = DirectX::XMVector3Normalize(vPlaneN);
			DirectX::XMStoreFloat3(&planeN, vPlaneN);

			plane = { planeN, vtx.pos[3] };

			triangle[0] = { vtx.pos[3],vtx.pos[1],vtx.pos[7] };
			triangle[1] = { vtx.pos[1],vtx.pos[5],vtx.pos[7] };


			resultPlayer = Collision::CheckCirclePlane(m_pPlayer->GetPos(), 0.5f, plane, triangle);
			if (resultPlayer.hit)
			{
				m_pPlayer->Landing(resultPlayer.point, plane.normal);
			}

			if (m_pEnemy[i] != nullptr)
			{
				resultEnemy = Collision::CheckCirclePlane(m_pEnemy[i]->GetPos(), 1.5f, plane, triangle);
				if (resultEnemy.hit)
				{
					DirectX::XMFLOAT3 out = calcReflectVector(m_pEnemy[i]->GetVector(), planeN);
					m_pEnemy[i]->Landing(resultEnemy.point, plane.normal, out);
				}
			}
			// 裏面
			planeN = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

			vPlaneN = DirectX::XMLoadFloat3(&planeN);
			Rx = DirectX::XMMatrixRotationX(block.rot.x);
			Ry = DirectX::XMMatrixRotationY(block.rot.y);
			Rz = DirectX::XMMatrixRotationZ(block.rot.z);
			vPlaneN = DirectX::XMVector3TransformCoord(vPlaneN, Rx * Ry * Rz);
			vPlaneN = DirectX::XMVector3Normalize(vPlaneN);
			DirectX::XMStoreFloat3(&planeN, vPlaneN);

			plane = { planeN, vtx.pos[1] };

			triangle[0] = { vtx.pos[1],vtx.pos[0],vtx.pos[5] };
			triangle[1] = { vtx.pos[0],vtx.pos[4],vtx.pos[5] };


			resultPlayer = Collision::CheckCirclePlane(m_pPlayer->GetPos(), 0.5f, plane, triangle);
			if (resultPlayer.hit)
			{
				m_pPlayer->Landing(resultPlayer.point, plane.normal);
			}

			if (m_pEnemy[i] != nullptr)
			{
				resultEnemy = Collision::CheckCirclePlane(m_pEnemy[i]->GetPos(), 1.5f, plane, triangle);
				if (resultEnemy.hit)
				{
					DirectX::XMFLOAT3 out = calcReflectVector(m_pEnemy[i]->GetVector(), planeN);
					m_pEnemy[i]->Landing(resultEnemy.point, plane.normal, out);
				}
			}
		}
	}

	if (Hit)
	{
	}
	//エネミー追従
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (m_pEnemy[i] != nullptr && !m_pEnemy[i]->CheckTouch())
		{
			fEnemyPos.x =
				m_pEnemy[i]->GetPos().x +
				(m_pPlayer->GetPos().x - m_pEnemy[i]->GetPos().x) *
				((float)m_pEnemy[i]->GetCurrent() / m_MaxCurrent);
			fEnemyPos.z =
				m_pEnemy[i]->GetPos().z +
				(m_pPlayer->GetPos().z - m_pEnemy[i]->GetPos().z) *
				((float)m_pEnemy[i]->GetCurrent() / m_MaxCurrent);

			m_pEnemy[i]->SetPos(fEnemyPos.x, 0.5f, fEnemyPos.z);
		}
	}
}
void Game3D::Draw()
{
	m_pStage->Draw();
	
	m_pPlayer->Draw();
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (m_pEnemy[i] != nullptr)
		{
			m_pEnemy[i]->Draw();
		}
	}

	if(m_pBOSS)
	m_pBOSS->Draw();
	if(m_pBullet)
	m_pBullet->Draw();

	SetGeometoryVPMatrix(m_pCamera[m_mainCamera]->GetViewMatrix(), m_pCamera[m_mainCamera]->GetProjectionMatrix());

	EnableDepth(false);	// 2D化
	m_pGameUI->Draw();
	m_pUI->Draw();
	EnableDepth(true); // 3D化
}

//////////////////////////////////
// 反射ベクトル
//
//	公式　r = f - 2 * Dot( f, n ) * n
//
// out : 正規化反射ベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
//////////////////////////////////
DirectX::XMFLOAT3 Game3D::calcReflectVector(const DirectX::XMFLOAT3& front, const DirectX::XMFLOAT3& normal) {
	DirectX::XMVECTOR normal_n;
	DirectX::XMVECTOR vFront = DirectX::XMLoadFloat3(&front);
	DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&normal);
	normal_n = DirectX::XMVector3Normalize(vNormal);

	DirectX::XMFLOAT3 temp;
	DirectX::XMStoreFloat3(&temp, DirectX::XMVectorScale(
		DirectX::XMVector3Dot(vFront, normal_n), 2.0f));

	DirectX::XMVECTOR vOut = DirectX::XMVectorSubtract(vFront,
		DirectX::XMVectorScale(normal_n, temp.x));

	vOut = DirectX::XMVector3Normalize(vOut);
	DirectX::XMFLOAT3 out;
	DirectX::XMStoreFloat3(&out, vOut);
	return out;
}

void Game3D::RespawnEnemy()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!m_pEnemy[i])
		{
			m_pEnemy[i] = new Enemy(m_pModelList->GetModelList(MODEL_PLAYER));
			m_pEnemy[i]->SetPos((rand() % LIMIT_POS_RIGHT_X) - (rand() % LIMIT_POS_LEFT_X),
				0.5f,
				(rand() % LIMIT_POS_FRONT_Z) - (rand() % LIMIT_POS_BACK_Z));
			//エネミーのカメラセット
			m_pEnemy[i]->SetCamera(m_pCamera[m_mainCamera]);
		}
	}
}

void Game3D::AttackEnemy()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (m_pEnemy[i] != nullptr)
		{
			// プレイヤーのベクトル計算
			DirectX::XMFLOAT3 camPos = m_pCamera[CAM_PLAYER]->GetPos();
			DirectX::XMFLOAT3 camLook = m_pCamera[CAM_PLAYER]->GetLook();
			camPos.y = 0.0f;
			camLook.y = 0.0f;
			DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
			DirectX::XMVECTOR vCamLook = DirectX::XMLoadFloat3(&camLook);
			DirectX::XMVECTOR vFront;
			vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
			vFront = DirectX::XMVector3Normalize(vFront);

			// 敵に向かうベクトル計算
			DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();
			DirectX::XMFLOAT3 EnemyPos = m_pEnemy[i]->GetPos();
			PlayerPos.y = 0.0f;
			EnemyPos.y = 0.0f;
			DirectX::XMVECTOR vPlayerPos = DirectX::XMLoadFloat3(&PlayerPos);
			DirectX::XMVECTOR vEnemyPos = DirectX::XMLoadFloat3(&EnemyPos);

			// 正規化された自分から敵に向かうベクトル
			DirectX::XMVECTOR vFrontEnemy = DirectX::XMVectorSubtract(vEnemyPos, vPlayerPos);
			vFrontEnemy = DirectX::XMVector3Normalize(vFrontEnemy);

			float angle;
			// 内積計算
			DirectX::XMVECTOR inner = DirectX::XMVector3Dot(vFront, vFrontEnemy);
			DirectX::XMStoreFloat(&angle, inner);

			if (IsKeyTrigger(VK_SHIFT))
			{
				if ((m_pPlayer->GetPos().x - m_pEnemy[i]->GetPos().x) * (m_pPlayer->GetPos().x - m_pEnemy[i]->GetPos().x) +
					(m_pPlayer->GetPos().y - m_pEnemy[i]->GetPos().y) * (m_pPlayer->GetPos().y - m_pEnemy[i]->GetPos().y) +
					(m_pPlayer->GetPos().z - m_pEnemy[i]->GetPos().z) * (m_pPlayer->GetPos().z - m_pEnemy[i]->GetPos().z)
					<= (0.5f + 0.5f) * (0.5f + 0.5f) && angle > 0.0f)
				{
					m_pEnemy[i]->Touch();
					m_pEnemy[i]->Collision(m_pPlayer->GetPos());
					m_pEnemy[i]->TochtEnemy();
				}
			}
		}
	}
}

void Game3D::CollisionEnemy()
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if(m_pEnemy[i] != nullptr)
		{
			if ((m_pPlayer->GetPos().x - m_pEnemy[i]->GetPos().x) * (m_pPlayer->GetPos().x - m_pEnemy[i]->GetPos().x) +
				(m_pPlayer->GetPos().y - m_pEnemy[i]->GetPos().y) * (m_pPlayer->GetPos().y - m_pEnemy[i]->GetPos().y) +
				(m_pPlayer->GetPos().z - m_pEnemy[i]->GetPos().z) * (m_pPlayer->GetPos().z - m_pEnemy[i]->GetPos().z)
				<= (0.5f + 0.5f) * (0.5f + 0.5f))
			{
				m_pGameUI->Damage();
			}
		}
	}
}

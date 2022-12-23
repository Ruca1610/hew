#include "Stage.h"
#include "Geometory.h"

Stage::Stage()
	:m_pBlocks(nullptr)
	, m_blockNum(9)
{
	m_pBlocks = new Info[m_blockNum];
	//床
	m_pBlocks[0].pos = { 0.0f,0.0f,0.0f };
	m_pBlocks[0].size = { 50.0f,1.0f,50.0f };
	m_pBlocks[0].rot = { 0.0f,0.0f,0.0f };

	//後ろ壁
	m_pBlocks[1].pos = { 0.0f,5.0f,-23.5f };
	m_pBlocks[1].size = { 20.0f,10.0f,1.0f };
	m_pBlocks[1].rot = { 0.0f,0.0f,0.0f };
	//前壁
	m_pBlocks[2].pos = { 0.0f,5.0f,23.5f };
	m_pBlocks[2].size = { 20.0f,10.0f,1.0f };
	m_pBlocks[2].rot = { 0.0f,0.0f,0.0f };

	//右壁
	m_pBlocks[3].pos = { 23.5f,5.0f,0.0f };
	m_pBlocks[3].size = { 1.0f,10.0f,20.0f };
	m_pBlocks[3].rot = { 0.0f,0.0f,0.0f };
	//左壁
	m_pBlocks[4].pos = { -23.5f,5.0f,0.0f };
	m_pBlocks[4].size = { 1.0f,10.0f,20.0f };
	m_pBlocks[4].rot = { 0.0f,0.0f,0.0f };

	//正面右斜め壁
	m_pBlocks[5].pos = { 16.5f,5.0f,16.5f };
	m_pBlocks[5].size = { 1.0f,10.0f,20.0f };
	m_pBlocks[5].rot = { 0.0f,-45.0f * 3.14f / 180.0f,0.0f };
	//正面左前壁
	m_pBlocks[6].pos = { -16.5f,5.0f,16.5f };
	m_pBlocks[6].size = { 1.0f,10.0f,20.0f };
	m_pBlocks[6].rot = { 0.0f,45.0f * 3.14f / 180.0f,0.0f };

	//後ろ右下斜め壁
	m_pBlocks[7].pos = { 16.5f,5.0f,-16.5f };
	m_pBlocks[7].size = { 1.0f,10.0f,20.0f };
	m_pBlocks[7].rot = { 0.0f,45.0f * 3.14f / 180.0f,0.0f };
	//後ろ右斜め壁
	m_pBlocks[8].pos = { -16.5f,5.0f,-16.5f };
	m_pBlocks[8].size = { 1.0f,10.0f,20.0f };
	m_pBlocks[8].rot = { 0.0f,-45.0f * 3.14f / 180.0f,0.0f };
}

Stage::~Stage()
{
	delete[] m_pBlocks;
}

void Stage::Draw()
{
	for (int i = 0; i < m_blockNum; ++i)
	{
		SetGeometoryTranslate(m_pBlocks[i].pos.x, m_pBlocks[i].pos.y, m_pBlocks[i].pos.z);
		SetGeometoryScaling(m_pBlocks[i].size.x, m_pBlocks[i].size.y, m_pBlocks[i].size.z);
		SetGeometoryRotation(m_pBlocks[i].rot.x, m_pBlocks[i].rot.y, m_pBlocks[i].rot.z);
		DrawBox();
	}
}

Stage::Info Stage::GetBlockInfo(unsigned int index)
{
	return m_pBlocks[index];
}

int Stage::GetBlockNum()
{
	return m_blockNum;
}

Stage::Vertices Stage::GetBlockVertices(unsigned int index)
{
	Vertices vtx = {};
	DirectX::XMFLOAT3 stride[] = {
	DirectX::XMFLOAT3(-1.0f,1.0f,1.0f),DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f),
	DirectX::XMFLOAT3(1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(1.0f,-1.0f,1.0f),
	DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f)
	};
	/*ブロックの中心座標*/
	DirectX::XMVECTOR vCenter = DirectX::XMLoadFloat3(&m_pBlocks[index].pos);
	DirectX::XMFLOAT3 half = DirectX::XMFLOAT3(
		m_pBlocks[index].size.x*0.5f, m_pBlocks[index].size.y*0.5f, m_pBlocks[index].size.z*0.5f
	);
	/*ブロックの回転行列*/
	DirectX::XMMATRIX mRot;
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_pBlocks[index].rot.x);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_pBlocks[index].rot.y);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_pBlocks[index].rot.z);
	mRot = Rx * Ry*Rz;

	for (int i = 0; i < 8; ++i)
	{
		//各隅の中心からの距離を計算
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(
			half.x*stride[i].x, half.y*stride[i].y, half.z*stride[i].z, 0.0f
		);
		//ブロックの八隅を回転に合わせて移動させる
		pos = DirectX::XMVector3TransformCoord(pos, mRot);
		//ブロックを回転させてから、中心座標まで移動させる
		DirectX::XMStoreFloat3(&vtx.pos[i], DirectX::XMVectorAdd(pos, vCenter));
	}
	return vtx;
}

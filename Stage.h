#pragma once
#include <DirectXMath.h>
class Stage
{
public:
	struct Info
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 size;
		DirectX::XMFLOAT3 rot;
	};
	Stage();
	~Stage();
	void Draw();

	struct Vertices
	{
		DirectX::XMFLOAT3 pos[8];
	};
	
	Info GetBlockInfo(unsigned int index);
	int GetBlockNum();
	Vertices GetBlockVertices(unsigned int index);

private:
	Info* m_pBlocks;
	int m_blockNum;
};
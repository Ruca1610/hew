#include "Geometory.h"
#include "MeshBuffer.h"
#include "Shader.h"
#include "ConstantBuffer.h"

//--- マクロ
#define GEOMETORY_SAFE_DELETE(p) if(p){delete p; p = nullptr;}

//--- 定数
const int GEOMETORY_SPLIT = 16;
const int GEOMETORY_SPLIT_VTX_X = GEOMETORY_SPLIT + 1;
const int GEOMETORY_SPLIT_VTX_Y = GEOMETORY_SPLIT / 2 + 1;
const int GEOMETORY_SPLIT_IDX_X = GEOMETORY_SPLIT;
const int GEOMETORY_SPLIT_IDX_Y = GEOMETORY_SPLIT / 2;

//--- 構造体
struct GeometoryVertex
{
	float pos[3];
	float uv[2];
};

//--- プロトタイプ宣言
void CreateGeometoryBox();
void CreateGeometorySphere();
void CreateGeometoryCylinder();
void CreateGeometoryCapsule();
void CreateGeometoryCone();
void CreateGeometoryShader();
void CreateGeometoryConstantBuffer();
void UpdateGeometoryMatrix();

//--- グローバル変数
MeshBuffer* g_pGeometoryBox;
MeshBuffer* g_pGeometorySphere;
MeshBuffer* g_pGeometoryCylinder;
MeshBuffer* g_pGeometoryCapsule;
MeshBuffer* g_pGeometoryCone;
VertexShader* g_pGeometoryVS;
PixelShader* g_pGeometoryPS;
ConstantBuffer* g_pGeometoryWVP;
DirectX::XMFLOAT4X4 g_geometoryMat[3];
DirectX::XMFLOAT3 g_geometoryTransform[3];

HRESULT InitGeometory()
{
	CreateGeometoryShader();
	CreateGeometoryConstantBuffer();
	CreateGeometoryBox();
	CreateGeometorySphere();
	CreateGeometoryCylinder();
	CreateGeometoryCapsule();
	CreateGeometoryCone();
	return S_OK;
}
void UninitGeometory()
{
	GEOMETORY_SAFE_DELETE(g_pGeometoryCone);
	GEOMETORY_SAFE_DELETE(g_pGeometoryCapsule);
	GEOMETORY_SAFE_DELETE(g_pGeometoryCylinder);
	GEOMETORY_SAFE_DELETE(g_pGeometorySphere);
	GEOMETORY_SAFE_DELETE(g_pGeometoryBox);
	GEOMETORY_SAFE_DELETE(g_pGeometoryPS);
	GEOMETORY_SAFE_DELETE(g_pGeometoryVS);
}
void SetGeometoryTranslate(float x, float y, float z)
{
	g_geometoryTransform[0] = DirectX::XMFLOAT3(x, y, z);
}
void SetGeometoryRotation(float x, float y, float z)
{
	g_geometoryTransform[1] = DirectX::XMFLOAT3(x, y, z);
}
void SetGeometoryScaling(float x, float y, float z)
{
	g_geometoryTransform[2] = DirectX::XMFLOAT3(x, y, z);
}
void SetGeometoryVPMatrix(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{
	g_geometoryMat[1]=view;
	g_geometoryMat[2]=proj;
}
void DrawBox()
{
	UpdateGeometoryMatrix();
	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryWVP->BindVS(0);
	g_pGeometoryBox->Draw();
}
void DrawSphere()
{
	UpdateGeometoryMatrix();
	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryWVP->BindVS(0);
	g_pGeometorySphere->Draw();
}
void DrawCylinder()
{
	UpdateGeometoryMatrix();
	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryWVP->BindVS(0);
	g_pGeometoryCylinder->Draw();
}
void DrawCone()
{
	UpdateGeometoryMatrix();
	g_pGeometoryVS->Bind();
	g_pGeometoryPS->Bind();
	g_pGeometoryWVP->BindVS(0);
	g_pGeometoryCone->Draw();
}
void DrawCapsule()
{
}
void DrawArrow()
{
}



void CreateGeometoryBox()
{
	GeometoryVertex vtx[] = {
		//-Z面
		{{-0.5f, 0.5f,-0.5f},{0.0f,0.0f}},
		{{ 0.5f, 0.5f,-0.5f},{1.0f,0.0f}},
		{{-0.5f,-0.5f,-0.5f},{0.0f,1.0f}},
		{{ 0.5f,-0.5f,-0.5f},{1.0f,1.0f}},
		//+Z面
		{{ 0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
		{{-0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
		{{ 0.5f,-0.5f, 0.5f},{0.0f,1.0f}},
		{{-0.5f,-0.5f, 0.5f},{1.0f,1.0f}},
		//-X面
		{{-0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
		{{-0.5f, 0.5f,-0.5f},{1.0f,0.0f}},
		{{-0.5f,-0.5f, 0.5f},{0.0f,1.0f}},
		{{-0.5f,-0.5f,-0.5f},{1.0f,1.0f}},
		//+X面
		{{ 0.5f, 0.5f,-0.5f},{0.0f,0.0f}},
		{{ 0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
		{{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f}},
		{{ 0.5f,-0.5f, 0.5f},{1.0f,1.0f}},
		//-Y面
		{{ 0.5f,-0.5f, 0.5f},{0.0f,0.0f}},
		{{-0.5f,-0.5f, 0.5f},{1.0f,0.0f}},
		{{ 0.5f,-0.5f,-0.5f},{0.0f,1.0f}},
		{{-0.5f,-0.5f,-0.5f},{1.0f,1.0f}},
		//+Y面
		{{-0.5f, 0.5f, 0.5f},{0.0f,0.0f}},
		{{ 0.5f, 0.5f, 0.5f},{1.0f,0.0f}},
		{{-0.5f, 0.5f,-0.5f},{0.0f,1.0f}},
		{{ 0.5f, 0.5f,-0.5f},{1.0f,1.0f}},
	};
	int idx[] = {
		0,1,2, 2,1,3,	//-Z面
		4,5,6, 6,5,7,	//+Z面
		8,9,10,10,9,11,	//-X面
		12,13,14, 14,13,15,	//+X面
		16,17,18, 18,17,19,	//-Y面
		20,21,22, 22,21,23,	//+Y面

	};
	MeshBuffer::Description desc = {};
	desc.pVtx		= vtx;
	desc.vtxCount	= 24;
	desc.vtxSize = 20;
	desc.pIdx = idx;
	desc.idxCount = 36;
	desc.idxSize = 4;
	desc.topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	g_pGeometoryBox = new MeshBuffer(desc);
}
void CreateGeometorySphere()
{
}
void CreateGeometoryCylinder()
{
}
void CreateGeometoryCone()
{
}
void CreateGeometoryCapsule()
{
}

void CreateGeometoryShader()
{
	const char* GeometoryVS = R"EOT(
struct VS_IN {
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct VS_OUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
};
cbuffer WVP : register(b0) {
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
VS_OUT main(VS_IN vin) {
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.wPos = vout.pos;
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	return vout;
})EOT";
	const char* GeometoryPS = R"EOT(
struct PS_IN {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;	
};
float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1,1,1,1);
	
	float2 mGrid = floor(abs(pin.uv) * 2.0f);
	float2 sGrid = floor(abs(pin.uv) * 8.0f);

	float mid = fmod(mGrid.x + mGrid.y, 2.0f);
	float small = fmod(sGrid.x + sGrid.y, 2.0f);

	color.rgb = ((mid * 0.1f) * small + 0.45f) + (1 - small) * 0.05f;
	return color;
})EOT";

	g_pGeometoryVS = new VertexShader();
	_ASSERT_EXPR(SUCCEEDED(g_pGeometoryVS->Compile(GeometoryVS)),
		L"create failed geometory vertex shader.");
	g_pGeometoryPS = new PixelShader();
	_ASSERT_EXPR(SUCCEEDED(g_pGeometoryPS->Compile(GeometoryPS)),
		L"create failed geometory pixel shader.");
}
void CreateGeometoryConstantBuffer()
{
	g_pGeometoryWVP = new ConstantBuffer();
	g_pGeometoryWVP->Create(sizeof(g_geometoryMat));

	g_geometoryTransform[0] = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_geometoryTransform[1] = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_geometoryTransform[2] = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	/*SetGeometoryVPMatrix(
		DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(3, 3, -3, 1), DirectX::XMVectorSet(0, 0, 0, 1), DirectX::XMVectorSet(0, 1, 0, 1)),
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.f), 16.f / 9.f, 0.1f, 100.0f)
	);*/
	DirectX::XMStoreFloat4x4(&g_geometoryMat[0], DirectX::XMMatrixIdentity());
	g_pGeometoryWVP->Write(g_geometoryMat);
}
void UpdateGeometoryMatrix()
{
	/*g_geometoryTransform[1].x += 0.02f;
	g_geometoryTransform[1].y += 0.02f;
	g_geometoryTransform[1].z += 0.02f;*/
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(g_geometoryTransform[0].x, g_geometoryTransform[0].y, g_geometoryTransform[0].z);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(g_geometoryTransform[1].x);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(g_geometoryTransform[1].y);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(g_geometoryTransform[1].z);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(g_geometoryTransform[2].x, g_geometoryTransform[2].y, g_geometoryTransform[2].z);
	DirectX::XMMATRIX mat = S * Rx * Ry * Rz * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMStoreFloat4x4(&g_geometoryMat[0], mat);
	g_pGeometoryWVP->Write(g_geometoryMat);
}
#pragma once
#include <DirectXMath.h>

class Collision
{
public:
	//--- 当たり判定を行う形状
	struct Plane
	{
		DirectX::XMFLOAT3 normal;	// 面の法線
		DirectX::XMFLOAT3 pos;		// 面上の座標
	};
	struct Ray
	{
		DirectX::XMFLOAT3 start;	// レイの開始点
		DirectX::XMFLOAT3 direction;// レイの方向
		float length;				// レイの長さ
	};

	//--- 当たり判定の結果
	struct Result
	{
		bool hit;
		DirectX::XMFLOAT3 point;
	};

	struct Triangle
	{
		DirectX::XMFLOAT3 p[3];	// 三角形の座標
	};

	//--- 形状ごとの当たり判定を実装
	static Result CheckRayPlane(Ray ray, Plane plane);

	static Result CheckCirclePlane(DirectX::XMFLOAT3 CircleCenter, float r, Plane plane, Collision::Triangle triangle[2]);

	static Result CheckPointTriangle(DirectX::XMFLOAT3 point, Triangle triangle);
private:
	DirectX::XMFLOAT3 CircleCenter;		//球の中心座標(プレイヤーの座標)
	float r;							//球の半径(適当)
	Plane plane;						//面の情報(plane)
	Triangle triangle[2];	//2つの三角形の頂点(triangle[2])
};
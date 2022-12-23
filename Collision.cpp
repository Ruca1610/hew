#include "Collision.h"

Collision::Result Collision::CheckRayPlane(Ray ray, Plane plane)
{
	Result result = {};

	DirectX::XMVECTOR vRayStart = DirectX::XMLoadFloat3(&ray.start);
	DirectX::XMVECTOR vRayN = DirectX::XMLoadFloat3(&ray.direction);
	DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&plane.normal);
	DirectX::XMVECTOR vPlanePos = DirectX::XMLoadFloat3(&plane.pos);

	vRayN = DirectX::XMVector3Normalize(vRayN);
	vPlaneN = DirectX::XMVector3Normalize(vPlaneN);

	//---平面から始点までの距離
	float p1;
	DirectX::XMVECTOR vToStart = DirectX::XMVectorSubtract(vRayStart, vPlanePos);	//平面からレイの始点までのベクトル
	{
		DirectX::XMVECTOR vTemp = DirectX::XMVector3Dot(vPlaneN, vToStart);	//面の法線と始点に向かうベクトルとの内積の結果を計算
		DirectX::XMStoreFloat(&p1, vTemp);									//計算結果をp1へ格納(正の僅かの判定や、表面の比率に使用
	}

	//---終点から平面までの距離
	float p2;
	DirectX::XMVECTOR vRay = DirectX::XMVectorScale(vRayN, ray.length);

	DirectX::XMVECTOR vToEnd = DirectX::XMVectorAdd(vRay, vToStart);	//平面からレイの終点までのベクトル

	DirectX::XMVECTOR vTemp = DirectX::XMVector3Dot(vPlaneN, vToEnd);	//面の法線と終点に向かうベクトルとの内積の結果を計算
	DirectX::XMStoreFloat(&p2, vTemp);									//計算結果をp2へ格納(負の僅かの判定や、裏面の比率に使用

	//---レイが面を貫通しているか
	if (p1 >= 0 && p2 <= 0)
	{
		result.hit = true;
		float dist;
		//レイの始点から貫通しているか位置までの距離を計算
		//レイのながさに内積に比率を掛ける(レイの長さ * (p1 / (p1 + (-p2)))
		dist = ray.length*(p1 / (p1 + (-p2)));

		//比率を掛けたものを、レイの単位ベクトルに掛ける
		DirectX::XMVECTOR vTemp = DirectX::XMVectorScale(vRayN, dist);

		//レイの始点に、比重を掛けたベクトルを加算する
		vTemp = DirectX::XMVectorAdd(vRayStart, vTemp);

		//計算結果をresult.pointへ格納
		DirectX::XMStoreFloat3(&result.point, vTemp);
	}
	return result;
}

Collision::Result Collision::CheckCirclePlane(DirectX::XMFLOAT3 CircleCenter, float r, Plane plane, Collision::Triangle triangle[2])
{
	Result result = {};

	DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&plane.normal);
	vPlaneN = DirectX::XMVector3Normalize(vPlaneN);
	DirectX::XMVECTOR vRayN = DirectX::XMVectorScale(vPlaneN, -1.0f);
	vRayN = DirectX::XMVector3Normalize(vRayN);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, vRayN);

	Ray ray = { CircleCenter,direction , r };
	result = Collision::CheckRayPlane(ray, plane);

	if (result.hit)
	{
		DirectX::XMFLOAT3 point = result.point;
		for (int i = 0; i < 2; i++)
		{
			result = Collision::CheckPointTriangle(point, triangle[i]);
			if (result.hit)
			{
				DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&result.point);
				DirectX::XMStoreFloat3(&result.point, DirectX::XMVectorAdd(DirectX::XMVectorScale(vPlaneN, r), vPoint));
				break;
			}
		}
	}

	return result;
}

Collision::Result Collision::CheckPointTriangle(DirectX::XMFLOAT3 point, Triangle triangle)
{
	//Result result = { false };
	DirectX::XMVECTOR vCross[3];	//外積の計算結果
	DirectX::XMVECTOR vPoint = DirectX::XMLoadFloat3(&point);			//引数pointを変換
	DirectX::XMVECTOR vTriStart = DirectX::XMLoadFloat3(&triangle.p[0]);//最初に処理する頂点を変換(triangle.p[0])

	for (int i = 0; i < 3; ++i)
	{
		DirectX::XMVECTOR vTriEnd = DirectX::XMLoadFloat3(&triangle.p[(i + 1) % 3]);	//辺の終点を変換(i=2の時は0が対象なので注意
		DirectX::XMVECTOR vToEdge = DirectX::XMVectorSubtract(vTriEnd, vTriStart);	//辺のベクトル
		DirectX::XMVECTOR vToPoint = DirectX::XMVectorSubtract(vPoint, vTriStart);	//点に向かうベクトル
		vCross[i] = DirectX::XMVector3Cross(vToEdge, vToPoint);						//vToEdgeとvToPointの外積を計算
		vCross[i] = DirectX::XMVector3Normalize(vCross[i]);
		vTriStart = vTriEnd;	//今の終点がつぎの始点になる
	}

	DirectX::XMVECTOR vDot[3] =
	{
		DirectX::XMVector3Dot(vCross[0],vCross[1]),
		DirectX::XMVector3Dot(vCross[1],vCross[2]),
		DirectX::XMVector3Dot(vCross[0],vCross[2])
	};
	float match[3];

	//---vDotの結果をmatchに変換
	for (int i = 0; i < 3; i++)
		DirectX::XMStoreFloat(&match[i], vDot[i]);

	Collision::Result result = { false,DirectX::XMFLOAT3(0.0f,0.0f,0.0f) };
	if ((match[0] >= 0.99f || match[0] == 0) &&
		(match[1] >= 0.99f || match[1] == 0) &&
		(match[2] >= 0.99f || match[2] == 0))
	{
		result.hit = true;
		result.point = point;
	}

	return result;

}

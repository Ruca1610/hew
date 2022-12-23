#pragma once
#include "CameraBase.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "Polyline.h"
#include "ModelList.h"

class Player
{
public:
	Player(static Model* ModelList);
	~Player();
	void Update();
	void Draw();

	void SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos();
	void Landing(DirectX::XMFLOAT3 hitpoint, DirectX::XMFLOAT3 planenormal);
private:
	DirectX::XMFLOAT3 m_pos;
	Model* m_pModel;
	VertexShader* m_pVS;
	ConstantBuffer* m_pWVP;
	CameraBase* m_pCamera;

	// âeÇÃèÓïÒ
	DirectX::XMFLOAT3 m_shadowPos;
	ID3D11ShaderResourceView* m_pShadowTex;

	// ÉoÅ[ÇÃèÓïÒ
	ID3D11ShaderResourceView* m_pBarTex;

	GeometoryPolyline* m_pPolyline;

	float m_gravity;
	float m_acc;

	bool m_CanJump;
};
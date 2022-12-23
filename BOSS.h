#pragma once
#include "CameraBase.h"
#include "Model.h"
#include "ConstantBuffer.h"

class BOSS
{
public:
	BOSS();
	~BOSS();
	void Update();
	void Draw();

	void SetCamera(CameraBase* pCamera);
	DirectX::XMFLOAT3 GetPos();




private:

	DirectX::XMFLOAT3 m_pos;
	Model* m_pModel;
	VertexShader* m_pVS;
	ConstantBuffer* m_pWVP;
	CameraBase* m_pCamera;


	//ÉÇÉfÉãì«Ç›çûÇ›Ç≥ÇÍÇƒÇ¢ÇÈÇ©
	static bool LoadModel;
	//---------------------
	DirectX::XMFLOAT3 PlayerPos;
};
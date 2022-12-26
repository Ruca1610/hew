//ボスのcppファイルです

#include "BOSS.h"
#include "Input.h"
#include <DirectXMath.h>


BOSS::BOSS()
	: m_pos(5.0f, 2.0f, 1.0f)
	, PlayerPos(m_pos)
	, m_pCamera(NULL)
{

	m_pModel = new Model();
	m_pModel->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);

	m_pVS = new VertexShader;
	if (FAILED(m_pVS->Load("Assets/Shader/ModelVS.cso")))
		MessageBox(nullptr, "ModelVS.cso", "Error", MB_OK);
	m_pModel->SetVertexShader(m_pVS);

	m_pWVP = new ConstantBuffer();//モデルを表示するために
	m_pWVP->Create(sizeof(DirectX::XMFLOAT4X4) * 3);


}

BOSS::~BOSS()
{

	delete m_pWVP;
	delete m_pVS;
	//delete m_pModel; //ModelListでdelete
}

void BOSS::Update()
{


}

void BOSS::Draw()
{
	if (!m_pCamera)
	{
		return;
	}
	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();
	m_pWVP->Write(mat);
	m_pWVP->BindVS(0);
	m_pModel->Draw();
}

void BOSS::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 BOSS::GetPos()
{
	DirectX::XMFLOAT3 pos = m_pos;
	pos.y += 0.1f;
	return pos;
}







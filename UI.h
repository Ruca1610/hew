#pragma once
#include "DirectX.h"

class UI
{
public:
	UI();
	~UI();
	void Draw();
private:
	ID3D11ShaderResourceView*	m_pBar;
};
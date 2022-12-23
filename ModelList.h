#pragma once
#include "Model.h"

enum E_MODELLIST
{
	MODEL_PLAYER = 0,
	MODEL_ENEMYZAKO,
	//MODEL_BOSS,

	MAX_MODEL
};


class ModelList
{
public:
	ModelList();
	~ModelList();
	Model* GetModelList(int);
private:
	Model* m_ModelList[MAX_MODEL];
};


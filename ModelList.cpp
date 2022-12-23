#include "ModelList.h"


ModelList::ModelList()
{

	m_ModelList[MODEL_PLAYER] = new Model();
	m_ModelList[MODEL_PLAYER]->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);

	m_ModelList[MODEL_ENEMYZAKO] = new Model();
	m_ModelList[MODEL_ENEMYZAKO]->Load("Assets/unitychan/unitychan.fbx", 0.01f, false);

}
ModelList::~ModelList()
{
	for (int i = 0; i < MAX_MODEL; i++)
	{
		if (!m_ModelList[i])	continue;

		delete m_ModelList[i];
	}
}
Model* ModelList::GetModelList (int i)
{
	return m_ModelList[i];
}

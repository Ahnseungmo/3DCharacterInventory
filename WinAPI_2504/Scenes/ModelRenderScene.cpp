#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
//	model = new Model("Character");
	
//	model = new Model("Naruto");
	model = new Model("LowPolyCharacter");
	//	model->SetShader(L"Model/ModelPalette.hlsl");
//	model->SetShader(L"Light/Light.hlsl");
//	model->SetShader();

}

ModelRenderScene::~ModelRenderScene()
{
	delete model;
}

void ModelRenderScene::Update()
{
	model->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	model->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	model->Edit();
}

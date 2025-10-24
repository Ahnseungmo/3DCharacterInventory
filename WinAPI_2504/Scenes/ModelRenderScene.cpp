#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	model = new Model("Character");
	
//	model->SetShader(L"Model/ModelPalette.hlsl");
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

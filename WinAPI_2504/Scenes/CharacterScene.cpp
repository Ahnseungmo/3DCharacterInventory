#include "Framework.h"
#include "CharacterScene.h"

CharacterScene::CharacterScene()
{
	naruto = new Naruto();
}

CharacterScene::~CharacterScene()
{
	delete naruto;
}

void CharacterScene::Update()
{
	naruto->Update();
}

void CharacterScene::PreRender()
{
}

void CharacterScene::Render()
{
	naruto->Render();
}

void CharacterScene::PostRender()
{
}

void CharacterScene::GUIRender()
{
	naruto->Edit();
}

#include "Framework.h"
#include "CharacterScene.h"

CharacterScene::CharacterScene()
{
	sword1 = new Sword();
	sword2 = new Sword();
	character = new Character();

	character->ChangeSword(sword1);
}

CharacterScene::~CharacterScene()
{
	delete character;
}

void CharacterScene::Update()
{
	if (Input::Get()->IsKeyDown('1')) {
		character->ChangeSword(sword1);
	}
	if (Input::Get()->IsKeyDown('2')) {
		character->ChangeSword(sword2);

	}

	character->Update();
}

void CharacterScene::PreRender()
{
}

void CharacterScene::Render()
{
	character->Render();
}

void CharacterScene::PostRender()
{
}

void CharacterScene::GUIRender()
{
	character->Edit();
}

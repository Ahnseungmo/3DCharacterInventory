#include "Framework.h"
#include "CharacterInventoryScene.h"

CharacterInventoryScene::CharacterInventoryScene()
{
	sword1 = new Sword();
	sword2 = new Sword();
	character = new Character();

	character->ChangeSword(sword1);
	inventoryManager = new InventoryManager("inventory.db");
}

CharacterInventoryScene::~CharacterInventoryScene()
{
	delete character;
	delete inventoryManager;
}

void CharacterInventoryScene::Update()
{
	if (Input::Get()->IsKeyDown('1')) {
		character->ChangeSword(sword1);
	}
	if (Input::Get()->IsKeyDown('2')) {
		character->ChangeSword(sword2);

	}

	character->Update();
}

void CharacterInventoryScene::PreRender()
{
}

void CharacterInventoryScene::Render()
{
	character->Render();
}

void CharacterInventoryScene::PostRender()
{
}

void CharacterInventoryScene::GUIRender()
{
	character->Edit();
	inventoryManager->GUIRender();

	if (ImGui::Button("save")) {
		inventoryManager->SaveData();
	}


}

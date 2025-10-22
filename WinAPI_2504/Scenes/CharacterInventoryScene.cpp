#include "Framework.h"
#include "CharacterInventoryScene.h"

CharacterInventoryScene::CharacterInventoryScene()
{
	naruto = new Naruto();
	inventoryManager = new InventoryManager("inventory.db");
}

CharacterInventoryScene::~CharacterInventoryScene()
{
	delete naruto;
	delete inventoryManager;
}

void CharacterInventoryScene::Update()
{
	naruto->Update();
}

void CharacterInventoryScene::PreRender()
{
}

void CharacterInventoryScene::Render()
{
	naruto->Render();
}

void CharacterInventoryScene::PostRender()
{
}

void CharacterInventoryScene::GUIRender()
{
	naruto->Edit();
	inventoryManager->GUIRender();

	if (ImGui::Button("save")) {
		inventoryManager->SaveData();
	}


}

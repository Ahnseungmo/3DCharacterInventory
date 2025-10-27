#pragma once

class CharacterInventoryScene : public Scene
{
public:
	CharacterInventoryScene();
	~CharacterInventoryScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Character* character;

	Sword* sword1;
	Sword* sword2;

	InventoryManager* inventoryManager;
};
#pragma once

#include "Framework.h"

struct ItemData {
    int id;
    string name;
    string type;
    string equip_slot; // ���� ����. ��������� ���� �Ұ�
    string description;
    // ImTextureID icon_texture; // ������ �ؽ�ó ID 
};

struct ItemSlot {
    int item_id = -1; // -1�� �� ����
    int quantity = 0;
    const ItemData* item_data = nullptr; // ������ ������ ���� ������
};

class InventoryManager
{
public:
    InventoryManager(string dbPath);
    ~InventoryManager();

    void GUIRender();
    void SaveData();

private:
    void InitializeDB();
    void LoadData();

    void RenderInventoryWindow();
    void RenderEquipmentWindow();

    const ItemData* GetItemData(int item_id);

private:
    string dbPath;
    sqlite3* db = nullptr;

    map<int, ItemData> all_items; // ���� �� ��� ������ ����
    vector<ItemSlot> inventory;   // �÷��̾� �κ��丮
    map<string, ItemSlot> equipment; // �÷��̾� ���
};
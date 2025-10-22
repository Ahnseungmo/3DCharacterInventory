#pragma once

#include "Framework.h"
#include "sqlite3.h"
#include <string>
#include <vector>
#include <map>

// ������ ������ ���� ����ü
struct ItemData {
    int id;
    string name;
    string type;
    string equip_slot; // ���� ����. ��������� ���� �Ұ�
    string description;
    // ImTextureID icon_texture; // ������ �ؽ�ó ID (���� �ε�)
};

// �κ��丮/��� ���� ���� ����ü
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

private:
    void InitializeDB();
    void LoadData();
    void SaveData();

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
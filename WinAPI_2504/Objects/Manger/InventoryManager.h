#pragma once

#include "Framework.h"
#include "sqlite3.h"
#include <string>
#include <vector>
#include <map>

// 아이템 마스터 정보 구조체
struct ItemData {
    int id;
    string name;
    string type;
    string equip_slot; // 장착 부위. 비어있으면 장착 불가
    string description;
    // ImTextureID icon_texture; // 아이콘 텍스처 ID (추후 로드)
};

// 인벤토리/장비 슬롯 정보 구조체
struct ItemSlot {
    int item_id = -1; // -1은 빈 슬롯
    int quantity = 0;
    const ItemData* item_data = nullptr; // 아이템 마스터 정보 포인터
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

    map<int, ItemData> all_items; // 게임 내 모든 아이템 정보
    vector<ItemSlot> inventory;   // 플레이어 인벤토리
    map<string, ItemSlot> equipment; // 플레이어 장비
};
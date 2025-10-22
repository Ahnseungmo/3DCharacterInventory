#include "Framework.h"
#include "InventoryManager.h"
#include <iostream>

InventoryManager::InventoryManager(string dbPath)
    : dbPath(dbPath)
{
    InitializeDB();
    LoadData();
}

InventoryManager::~InventoryManager()
{
    SaveData();
    if (db) {
        sqlite3_close(db);
    }
}

void InventoryManager::InitializeDB()
{
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    /*
    char* errMsg = nullptr;
    const char* create_sql =
        "CREATE TABLE IF NOT EXISTS Items ("
        "    id INTEGER PRIMARY KEY, name TEXT NOT NULL, type TEXT NOT NULL,"
        "    equip_slot TEXT, description TEXT, max_stack INTEGER DEFAULT 1"
        ");"
        "CREATE TABLE IF NOT EXISTS Inventory ("
        "    slot_index INTEGER PRIMARY KEY, item_id INTEGER, quantity INTEGER,"
        "    FOREIGN KEY (item_id) REFERENCES Items(id)"
        ");"
        "CREATE TABLE IF NOT EXISTS Equipment ("
        "    equip_slot TEXT PRIMARY KEY, item_id INTEGER,"
        "    FOREIGN KEY (item_id) REFERENCES Items(id)"
        ");"

        // --- 샘플 데이터 추가 ---
        "INSERT OR IGNORE INTO Items (id, name, type, equip_slot, description) VALUES "
        "(1, 'Sword', 'Weapon', 'Weapon', 'light one hand sword'),"
        "(2, 'rager Armor', 'Armor', 'Chest', 'Rager Armor.'),"
        "(3, 'Hp Potion', 'Potion', NULL, 'Heal Hp.');"
        "INSERT OR IGNORE INTO Inventory (slot_index, item_id, quantity) VALUES (0, 1, 1), (1, 3, 5);";


    if (sqlite3_exec(db, create_sql, 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    */
}

void InventoryManager::LoadData()
{
    // Load All Items
    all_items.clear();
    sqlite3_stmt* stmt;
    const char* sql_items = "SELECT id, name, type, equip_slot, description FROM Items;";
    if (sqlite3_prepare_v2(db, sql_items, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            ItemData item;
            item.id = sqlite3_column_int(stmt, 0);
            item.name = (const char*)sqlite3_column_text(stmt, 1);
            item.type = (const char*)sqlite3_column_text(stmt, 2);
            const char* slot = (const char*)sqlite3_column_text(stmt, 3);
            item.equip_slot = slot ? slot : "";
            item.description = (const char*)sqlite3_column_text(stmt, 4);
            all_items[item.id] = item;
        }
    }
    sqlite3_finalize(stmt);


    inventory.assign(20, ItemSlot()); // 20칸 인벤토리
    const char* sql_inv = "SELECT slot_index, item_id, quantity FROM Inventory;";
    if (sqlite3_prepare_v2(db, sql_inv, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int slot_index = sqlite3_column_int(stmt, 0);
            if (slot_index < inventory.size()) {
                inventory[slot_index].item_id = sqlite3_column_int(stmt, 1);
                inventory[slot_index].quantity = sqlite3_column_int(stmt, 2);
                inventory[slot_index].item_data = GetItemData(inventory[slot_index].item_id);
            }
        }
    }
    sqlite3_finalize(stmt);

    // Load Equipment
    equipment.clear();
    const char* sql_equip = "SELECT equip_slot, item_id FROM Equipment;";
    if (sqlite3_prepare_v2(db, sql_equip, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string slot_name = (const char*)sqlite3_column_text(stmt, 0);
            int item_id = sqlite3_column_int(stmt, 1);
            equipment[slot_name] = { item_id, 1, GetItemData(item_id) };
        }
    }
    sqlite3_finalize(stmt);
}

void InventoryManager::SaveData()
{
    sqlite3_stmt* stmt;

    // Save Inventory
    sqlite3_exec(db, "DELETE FROM Inventory;", 0, 0, 0);
    const char* sql_inv = "INSERT INTO Inventory (slot_index, item_id, quantity) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql_inv, -1, &stmt, 0) != SQLITE_OK) return;
    for (int i = 0; i < inventory.size(); ++i) {
        if (inventory[i].item_id != -1) {
            sqlite3_bind_int(stmt, 1, i);
            sqlite3_bind_int(stmt, 2, inventory[i].item_id);
            sqlite3_bind_int(stmt, 3, inventory[i].quantity);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
    }
    sqlite3_finalize(stmt);

    // Save Equipment
    sqlite3_exec(db, "DELETE FROM Equipment;", 0, 0, 0);
    const char* sql_equip = "INSERT INTO Equipment (equip_slot, item_id) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sql_equip, -1, &stmt, 0) != SQLITE_OK) return;

    for (map<string, ItemSlot>::const_iterator it = equipment.begin(); it != equipment.end(); ++it) {
        const string& slot = it->first;
        const ItemSlot& item_slot = it->second;
        if (item_slot.item_id != -1) {
            sqlite3_bind_text(stmt, 1, slot.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 2, item_slot.item_id);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
    }
    sqlite3_finalize(stmt);
}

const ItemData* InventoryManager::GetItemData(int item_id)
{
    auto it = all_items.find(item_id);
    return (it != all_items.end()) ? &it->second : nullptr;
}

void InventoryManager::GUIRender()
{
    RenderInventoryWindow();
    RenderEquipmentWindow();
}

void InventoryManager::RenderInventoryWindow()
{
    ImGui::Begin("Inventory");
    ImVec2 button_size(64, 64);
    int items_per_row = 5;

    for (int i = 0; i < inventory.size(); ++i) {
        ImGui::PushID(i);
        ItemSlot& slot = inventory[i];

        if (slot.item_id != -1) {
            ImGui::Button(slot.item_data->name.c_str(), button_size);
        }
        else {
            ImGui::Button("Empty", button_size);
        }

        // 아이템 드래그 시작
        if (slot.item_id != -1 && ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("INVENTORY_ITEM", &i, sizeof(int));
            ImGui::Text("%s", slot.item_data->name.c_str());
            ImGui::EndDragDropSource();
        }

        // 다른 슬롯으로 아이템 드롭
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INVENTORY_ITEM")) {
                int source_idx = *(const int*)payload->Data;
                swap(inventory[i], inventory[source_idx]);
            }
            // 장비창에서 해제하는 경우
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EQUIPMENT_ITEM")) {
                string* source_slot_ptr = (string*)payload->Data;
                if (inventory[i].item_id == -1) { // 빈 슬롯에만 해제 가능
                    swap(inventory[i], equipment[*source_slot_ptr]);
                }
            }
            ImGui::EndDragDropTarget();
        }

        // 툴팁 표시
        if (ImGui::IsItemHovered() && slot.item_id != -1) {
            ImGui::BeginTooltip();
            ImGui::Text("%s", slot.item_data->name.c_str());
            ImGui::Separator();
            ImGui::Text("%s", slot.item_data->description.c_str());
            if (slot.quantity > 1) {
                ImGui::Text("Count : %d", slot.quantity);
            }
            ImGui::EndTooltip();
        }

        ImGui::PopID();
        if ((i + 1) % items_per_row != 0) {
            ImGui::SameLine();
        }
    }
    ImGui::End();
}

void InventoryManager::RenderEquipmentWindow()
{
    ImGui::Begin("Equipment");
    ImVec2 button_size(80, 80);
    vector<string> slot_names = { "Weapon", "Helmet", "Chest", "Legs", "Boots" };

    for (size_t i = 0; i < slot_names.size(); ++i) {
        const string& slot_name = slot_names[i];
        ImGui::PushID(slot_name.c_str());
        ImGui::Text("%s:", slot_name.c_str());

        ItemSlot& slot = equipment[slot_name];

        if (slot.item_id != -1) {
            ImGui::Button(slot.item_data->name.c_str(), button_size);
        }
        else {
            ImGui::Button(slot_name.c_str(), button_size);
        }

        // 장비창에서 아이템 드래그 시작 (해제)
        if (slot.item_id != -1 && ImGui::BeginDragDropSource()) {
            static string dragged_slot;
            dragged_slot = slot_name;
            ImGui::SetDragDropPayload("EQUIPMENT_ITEM", &dragged_slot, sizeof(string));
            ImGui::Text("%s 해제", slot.item_data->name.c_str());
            ImGui::EndDragDropSource();
        }

        //인벤토리에서 아이템 드롭 (장착)
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INVENTORY_ITEM")) {
                int source_idx = *(const int*)payload->Data;
                ItemSlot& item_to_equip = inventory[source_idx];

                if (item_to_equip.item_data && item_to_equip.item_data->equip_slot == slot_name) {
                    // 장비 교체
                    swap(equipment[slot_name], inventory[source_idx]);
                }
            }
            ImGui::EndDragDropTarget();
        }

        // 툴팁
        if (ImGui::IsItemHovered() && slot.item_id != -1) {
            ImGui::BeginTooltip();
            ImGui::Text("%s", slot.item_data->name.c_str());
            ImGui::Separator();
            ImGui::Text("%s", slot.item_data->description.c_str());
            ImGui::EndTooltip();
        }

        ImGui::PopID();
    }
    ImGui::End();
}
#include "Inventory.h"

Inventory::Inventory() : maxSize(20) {
    items.push_back(Item(ItemType::LEMBAS_BREAD));
    items.push_back(Item(ItemType::LEMBAS_BREAD));
    items.push_back(Item(ItemType::MITHRIL_OIL));
}

bool Inventory::addItem(const Item& item) {
    if (isFull()) return false;
    items.push_back(item);
    return true;
}

bool Inventory::useItem(int index, Player& player) {
    if (index < 0 || index >= items.size()) return false;
    
    const Item& item = items[index];
    
    switch(item.getType()) {
        case ItemType::LEMBAS_BREAD:
            player.heal(item.getValue());
            break;
        case ItemType::ENT_DRAUGHT:
            player.modifyAttack(item.getValue());
            break;
        case ItemType::MITHRIL_OIL:
            player.modifyDefense(item.getValue());
            break;
    }
    
    removeItem(index);
    return true;
}

void Inventory::removeItem(int index) {
    if (index >= 0 && index < items.size()) {
        items.erase(items.begin() + index);
    }
}
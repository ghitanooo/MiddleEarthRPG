#include "Item.h"
#include "raylib.h"

Item::Item(ItemType t) : type(t) {
    switch(type) {
        case ItemType::LEMBAS_BREAD:
            name = "Lembas Bread";
            value = 40;
            break;
        case ItemType::ENT_DRAUGHT:
            name = "Ent-draught";
            value = 10;
            break;
        case ItemType::MITHRIL_OIL:
            name = "Mithril Oil";
            value = 5;
            break;
    }
}

Item Item::createRandomItem() {
    int randomType = GetRandomValue(0, 2);
    return Item(static_cast<ItemType>(randomType));
}
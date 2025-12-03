#pragma once
#include <string>

enum class ItemType {
    LEMBAS_BREAD,
    ENT_DRAUGHT,
    MITHRIL_OIL
};

class Item {
private:
    ItemType type;
    std::string name;
    int value;
    
public:
    Item(ItemType t);
    
    ItemType getType() const { return type; }
    std::string getName() const { return name; }
    int getValue() const { return value; }
    
    static Item createRandomItem();
};
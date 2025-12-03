#pragma once
#include "Item.h"
#include "Player.h"
#include <vector>

class Inventory {
private:
    std::vector<Item> items;
    const int maxSize = 20;
    
public:
    Inventory();
    
    bool addItem(const Item& item);
    bool useItem(int index, Player& player);
    void removeItem(int index);
    
    int getSize() const { return items.size(); }
    int getMaxSize() const { return maxSize; }
    const Item& getItem(int index) const { return items[index]; }
    bool isEmpty() const { return items.empty(); }
    bool isFull() const { return items.size() >= maxSize; }
};
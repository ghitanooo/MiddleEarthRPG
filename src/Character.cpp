#include "Character.h"
#include <algorithm>

Character::Character(std::string n, int hp, int atk, int def)
    : name(n), maxHp(hp), currentHp(hp), attack(atk), defense(def) {}

bool Character::isAlive() const {
    return currentHp > 0;
}

void Character::takeDamage(int damage) {
    currentHp = std::max(currentHp - damage, 0);
}

void Character::heal(int amount) {
    currentHp = std::min(currentHp + amount, maxHp);
}

int Character::calculateDamage(int enemyDefense) const {
    int netDamage = attack - enemyDefense;
    return std::max(netDamage, 1);  
}
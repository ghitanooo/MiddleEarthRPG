#pragma once
#include <string>

class Character {
protected:
    std::string name;
    int maxHp;
    int currentHp;
    int attack;
    int defense;
    
public:
    Character(std::string n, int hp, int atk, int def);
    virtual ~Character() = default;
    
    // Getters
    std::string getName() const { return name; }
    int getMaxHp() const { return maxHp; }
    int getCurrentHp() const { return currentHp; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    
    // Setters
    void setCurrentHp(int hp) { currentHp = hp; }
    void modifyAttack(int amount) { attack += amount; }
    void modifyDefense(int amount) { defense += amount; }
    
    // Methods
    bool isAlive() const;
    void takeDamage(int damage);
    void heal(int amount);
    int calculateDamage(int enemyDefense) const;
};
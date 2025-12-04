#pragma once
#include "Character.h"

enum class EnemyType {
    GOBLIN_SCOUT,
    ORC_WARRIOR,
    URUK_HAI,
    CAVE_TROLL,
    NAZGUL
};

class Enemy : public Character {
private:
    EnemyType type;
    int xpReward;
    
public:
    Enemy(EnemyType t);
    
    EnemyType getType() const { return type; }
    int getXpReward() const { return xpReward; }
    
    static Enemy* createRandomEnemy(int playerLevel);
    
private:
    static void getEnemyStats(EnemyType type, std::string& name, int& hp, int& atk, int& def, int& xp);
};
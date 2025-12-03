#include "Enemy.h"
#include "raylib.h"
#include <cstdlib>

Enemy::Enemy(EnemyType t) : Character("", 0, 0, 0), type(t), xpReward(0) {
    std::string enemyName;
    int hp, atk, def, xp;
    getEnemyStats(t, enemyName, hp, atk, def, xp);
    
    name = enemyName;
    maxHp = hp;
    currentHp = hp;
    attack = atk;
    defense = def;
    xpReward = xp;
}

void Enemy::getEnemyStats(EnemyType type, std::string& name, int& hp, int& atk, int& def, int& xp) {
    switch(type) {
        case EnemyType::GOBLIN_SCOUT:
            name = "Goblin Scout";
            hp = 40; atk = 12; def = 2; xp = 25;
            break;
        case EnemyType::ORC_WARRIOR:
            name = "Orc Warrior";
            hp = 60; atk = 18; def = 5; xp = 40;
            break;
        case EnemyType::URUK_HAI:
            name = "Uruk-hai";
            hp = 80; atk = 22; def = 7; xp = 60;
            break;
        case EnemyType::CAVE_TROLL:
            name = "Cave Troll";
            hp = 100; atk = 25; def = 10; xp = 80;
            break;
        case EnemyType::NAZGUL:
            name = "Nazgul";
            hp = 120; atk = 30; def = 8; xp = 100;
            break;
    }
}

Enemy* Enemy::createRandomEnemy(int playerLevel) {
    int maxTier = (playerLevel - 1) / 2;
    if (maxTier > 4) maxTier = 4;
    
    int randomTier = GetRandomValue(0, maxTier);
    EnemyType type = static_cast<EnemyType>(randomTier);
    
    return new Enemy(type);
}
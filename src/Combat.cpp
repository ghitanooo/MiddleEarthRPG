#include "Combat.h"
#include "raylib.h"

Combat::Combat() : maxLogEntries(6), playerTurn(true), enemyAttackDelay(1.0f), enemyAttackTimer(0.0f) {}

void Combat::playerAttack(Player& player, Enemy& enemy) {
    if (!playerTurn) return;
    
    int damage = player.calculateDamage(enemy.getDefense());
    enemy.takeDamage(damage);
    
    addLog(player.getName() + " attacks for " + std::to_string(damage) + " damage!", SKYBLUE);
    
    if (!enemy.isAlive()) {
        addLog(enemy.getName() + " defeated!", GREEN);
    } else {
        playerTurn = false;
    }
}

void Combat::enemyAttack(Player& player, Enemy& enemy) {
    int damage = enemy.calculateDamage(player.getDefense());
    addLog(enemy.getName() + " attacks for " + std::to_string(damage) + " damage!", RED);

    // ✅ Apply the damage to the player
    player.takeDamage(damage);
    playerTurn = true;
    enemyAttackTimer = 0.0f;
}



void Combat::updateTimer(float deltaTime) {
    if (!playerTurn) {
        enemyAttackTimer += deltaTime;
    }
}



void Combat::addLog(const std::string& message, Color color) {
    battleLog.push_back({message, color});
    
    if (battleLog.size() > maxLogEntries) {
        battleLog.erase(battleLog.begin());
    }
}

void Combat::reset() {
    battleLog.clear();
    playerTurn = true;
    enemyAttackTimer = 0.0f;
}
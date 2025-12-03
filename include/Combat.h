#pragma once
#include "Player.h"
#include "Enemy.h"
#include <string>
#include <vector>

struct CombatLog {
    std::string message;
    Color color;
};

class Combat {
private:
    std::vector<CombatLog> battleLog;
    const int maxLogEntries = 6;
    bool playerTurn;
    float enemyAttackTimer;
    const float enemyAttackDelay = 1.0f;
    
public:
    Combat();
    
    // Combat methods
    void playerAttack(Player& player, Enemy& enemy);
    void enemyAttack(Player& player, Enemy& enemy);
    void reset();
    
    // Log management
    void addLog(const std::string& message, Color color = RAYWHITE);
    const std::vector<CombatLog>& getLog() const { return battleLog; }
    
    // Timer
    void updateTimer(float deltaTime);
    bool isEnemyTurn() const { return !playerTurn; }
    void setPlayerTurn(bool turn) { playerTurn = turn; }
};
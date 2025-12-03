#pragma once
#include "Player.h"
#include "raylib.h"
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
    int maxLogEntries;
    bool playerTurn;
    
public:
    float enemyAttackTimer;
    float enemyAttackDelay;
    
    Combat();
    
    void playerAttack(Player& player, Enemy& enemy);
    void enemyAttack(Player& player, Enemy& enemy);
    void reset();
    
    void addLog(const std::string& message, Color color = RAYWHITE);
    const std::vector<CombatLog>& getLog() const { return battleLog; }
    
    void updateTimer(float deltaTime);
   bool isEnemyTurn() const { return !playerTurn; }
   bool isPlayerTurn() const { return playerTurn; }
    void setPlayerTurn(bool turn) { playerTurn = turn; }
    };  
    
#pragma once
#include "raylib.h"
#include "GameState.h"
#include "Player.h"
#include "Enemy.h"
#include "Inventory.h"
#include "Combat.h"
#include <string>

class Game {
private:
    GameState currentState;
    Player* player;
    Enemy* currentEnemy;
    Inventory inventory;
    Combat combat;
    
    // UI state
    char playerName[21];
    CharacterClass selectedClass;
    int selectedInventorySlot;
    int enemiesDefeated;
    bool showLevelUpMessage;
    float levelUpTimer;
    
    // Input handling
    int nameCharCount;
    static const int maxNameLength = 20;
    //double battleEndTime = 0.0;
    
public:
    Game();
    ~Game();
    
    void Update();
    void Draw();
    
private:
    // Update methods
    void UpdateMenu();
    void UpdateClassSelect();
    void UpdateExploration();
    void UpdateCombat();
    void UpdateInventory();
    void UpdateGameOver();
    
    // Draw methods
    void DrawMenu();
    void DrawClassSelect();
    void DrawExploration();
    void DrawCombat();
    void DrawInventory();
    void DrawGameOver();
    
    // Helper methods
    void StartGame();
    void SpawnEnemy();
    void ResetGame();
    void DrawButton(Rectangle bounds, const char* text, bool hover);
    void DrawHealthBar(Rectangle bounds, int current, int max, Color color);
    void DrawCharacterStats(int x, int y, Character* character);
};
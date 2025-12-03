#include "Game.h"
#include <cstring>

Game::Game() 
    : currentState(GameState::MENU), 
      player(nullptr), 
      currentEnemy(nullptr),
      selectedClass(CharacterClass::WIZARD),
      selectedInventorySlot(-1),
      enemiesDefeated(0),
      showLevelUpMessage(false),
      levelUpTimer(0.0f),
      nameCharCount(0) {
    
    memset(&playerName[0], 0, sizeof(playerName));
}

Game::~Game() {
    delete player;
    delete currentEnemy;
}

void Game::Update() {
    switch(currentState) {
        case GameState::MENU:
            UpdateMenu();
            break;
        case GameState::CLASS_SELECT:
            UpdateClassSelect();
            break;
        case GameState::EXPLORATION:
            UpdateExploration();
            break;
        case GameState::COMBAT:
            UpdateCombat();
            break;
        case GameState::INVENTORY:
            UpdateInventory();
            break;
        case GameState::GAME_OVER:
            UpdateGameOver();
            break;
    }
    
    if (showLevelUpMessage) {
        levelUpTimer -= GetFrameTime();
        if (levelUpTimer <= 0) showLevelUpMessage = false;
    }
}

void Game::Draw() {
    switch(currentState) {
        case GameState::MENU:
            DrawMenu();
            break;
        case GameState::CLASS_SELECT:
            DrawClassSelect();
            break;
        case GameState::EXPLORATION:
            DrawExploration();
            break;
        case GameState::COMBAT:
            DrawCombat();
            break;
        case GameState::INVENTORY:
            DrawInventory();
            break;
        case GameState::GAME_OVER:
            DrawGameOver();
            break;
    }
    
    if (showLevelUpMessage) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        const char* msg = "*** LEVEL UP! ***";
        int textWidth = MeasureText(msg, 60);
        DrawText(msg, GetScreenWidth()/2 - textWidth/2, GetScreenHeight()/2 - 30, 60, GOLD);
    }
}

// ===== UPDATE METHODS =====

void Game::UpdateMenu() {
    // Get keyboard input for name
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (nameCharCount < maxNameLength - 1)) {
            playerName[nameCharCount] = (char)key;
            playerName[nameCharCount + 1] = '\0';
            nameCharCount++;
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE)) {
        nameCharCount--;
        if (nameCharCount < 0) nameCharCount = 0;
        playerName[nameCharCount] = '\0';
    }
    
    if (IsKeyPressed(KEY_ENTER) && nameCharCount > 0) {
        currentState = GameState::CLASS_SELECT;
    }
}

void Game::UpdateClassSelect() {
    if (IsKeyPressed(KEY_ONE)) selectedClass = CharacterClass::WIZARD;
    if (IsKeyPressed(KEY_TWO)) selectedClass = CharacterClass::HOBBIT;
    if (IsKeyPressed(KEY_THREE)) selectedClass = CharacterClass::DWARF;
    if (IsKeyPressed(KEY_FOUR)) selectedClass = CharacterClass::ELF;
    if (IsKeyPressed(KEY_FIVE)) selectedClass = CharacterClass::RANGER;
    
    if (IsKeyPressed(KEY_ENTER)) {
        StartGame();
    }
}

void Game::UpdateExploration() {
    if (IsKeyPressed(KEY_B)) {
        SpawnEnemy();
    }
    
    if (IsKeyPressed(KEY_I)) {
        currentState = GameState::INVENTORY;
    }
}

void Game::UpdateCombat() {
    combat.updateTimer(GetFrameTime());
    
    if (combat.isEnemyTurn() && combat.enemyAttackTimer >= combat.enemyAttackDelay) {
        combat.enemyAttack(*player, *currentEnemy);
        
        if (!player->isAlive()) {
            currentState = GameState::GAME_OVER;
            return;
        }
    }
    
    if (combat.isEnemyTurn()) return;
    
    if (IsKeyPressed(KEY_A)) {
        combat.playerAttack(*player, *currentEnemy);
        
        if (!currentEnemy->isAlive()) {
            int oldLevel = player->getLevel();
            player->gainExperience(currentEnemy->getXpReward());
            
            if (player->getLevel() > oldLevel) {
                showLevelUpMessage = true;
                levelUpTimer = 2.0f;
            }
            
            if (GetRandomValue(1, 100) <= 40) {
                Item loot = Item::createRandomItem();
                inventory.addItem(loot);
                combat.addLog("Found " + loot.getName() + "!", GOLD);
            }
            
            enemiesDefeated++;
            delete currentEnemy;
            currentEnemy = nullptr;
            currentState = GameState::EXPLORATION;
        }
    }
    
    if (IsKeyPressed(KEY_I) && combat.isPlayerTurn()) {
        currentState = GameState::INVENTORY;
    }
}

void Game::UpdateInventory() {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_I)) {
        selectedInventorySlot = -1;
        if (currentEnemy) {
            currentState = GameState::COMBAT;
        } else {
            currentState = GameState::EXPLORATION;
        }
        return;
    }
    
    for (int i = 0; i < inventory.getSize() && i < 9; i++) {
        if (IsKeyPressed(KEY_ONE + i)) {
            if (inventory.useItem(i, *player)) {
                combat.addLog("Used " + inventory.getItem(i).getName(), GREEN);
            }
        }
    }
}

void Game::UpdateGameOver() {
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }
}

// ===== DRAW METHODS =====

void Game::DrawMenu() {
    DrawText("JOURNEY THROUGH MIDDLE-EARTH", 200, 100, 40, GOLD);
    DrawText("A Mini RPG Adventure", 350, 150, 20, ORANGE);
    
    DrawRectangle(300, 250, 400, 60, DARKGRAY);
    DrawRectangleLines(300, 250, 400, 60, GOLD);
    DrawText("Enter your name:", 320, 220, 20, RAYWHITE);
    DrawText(playerName, 320, 265, 30, RAYWHITE);
    
    if (nameCharCount > 0) {
        DrawText("Press ENTER to continue", 360, 350, 20, GREEN);
    } else {
        DrawText("Type your hero's name...", 360, 350, 20, GRAY);
    }
}

void Game::DrawClassSelect() {
    DrawText("CHOOSE YOUR CLASS", 320, 80, 35, GOLD);
    
    const char* classes[] = {"[1] WIZARD", "[2] HOBBIT", "[3] DWARF", "[4] ELF", "[5] RANGER"};
    const char* stats[] = {
        "HP:80  ATK:25 DEF:3",
        "HP:100 ATK:15 DEF:8",
        "HP:120 ATK:20 DEF:10",
        "HP:90  ATK:22 DEF:5",
        "HP:110 ATK:18 DEF:7"
    };
    
    for (int i = 0; i < 5; i++) {
        Color color = (selectedClass == static_cast<CharacterClass>(i)) ? GOLD : RAYWHITE;
        DrawText(classes[i], 350, 180 + i * 60, 25, color);
        DrawText(stats[i], 370, 205 + i * 60, 18, GRAY);
    }
    
    DrawText("Press ENTER to start", 370, 600, 20, GREEN);
}

void Game::DrawExploration() {
    // Title
    DrawText("MIDDLE-EARTH", 400, 30, 30, GOLD);
    
    // Player stats
    DrawRectangle(50, 100, 300, 200, Fade(DARKGRAY, 0.8f));
    DrawRectangleLines(50, 100, 300, 200, GOLD);
    DrawText(player->getName().c_str(), 70, 120, 25, RAYWHITE);
    DrawText(TextFormat("Level %d %s", player->getLevel(), player->getClassName().c_str()), 70, 150, 18, GRAY);
    
    DrawHealthBar({70, 180, 250, 20}, player->getCurrentHp(), player->getMaxHp(), RED);
    DrawText(TextFormat("HP: %d/%d", player->getCurrentHp(), player->getMaxHp()), 75, 182, 16, WHITE);
    
    DrawText(TextFormat("ATK: %d  DEF: %d", player->getAttack(), player->getDefense()), 70, 210, 18, ORANGE);
    DrawText(TextFormat("XP: %d/%d", player->getExperience(), player->getXpToNextLevel()), 70, 235, 18, SKYBLUE);
    DrawText(TextFormat("Enemies Defeated: %d", enemiesDefeated), 70, 260, 18, GOLD);
    
    // Actions
    DrawRectangle(400, 300, 300, 100, Fade(DARKGRAY, 0.8f));
    DrawRectangleLines(400, 300, 300, 100, RED);
    DrawText("Press [B] to seek battle", 420, 330, 20, RED);
    
    DrawText("Press [I] for inventory", 400, 600, 18, GRAY);
}

void Game::DrawCombat() {
    DrawText("=== BATTLE ===", 420, 30, 30, RED);
    
    // Player side
    DrawRectangle(50, 100, 350, 250, Fade(DARKGREEN, 0.3f));
    DrawRectangleLines(50, 100, 350, 250, GREEN);
    DrawText(player->getName().c_str(), 70, 120, 22, GREEN);
    DrawText(player->getClassName().c_str(), 70, 145, 16, DARKGREEN);
    DrawCharacterStats(70, 170, player);
    
    // Enemy side
    DrawRectangle(600, 100, 350, 250, Fade(DARKPURPLE, 0.3f));
    DrawRectangleLines(600, 100, 350, 250, RED);
    DrawText(currentEnemy->getName().c_str(), 620, 120, 22, RED);
    DrawCharacterStats(620, 170, currentEnemy);
    
    // Combat log
    DrawRectangle(50, 400, 900, 200, Fade(BLACK, 0.8f));
    DrawRectangleLines(50, 400, 900, 200, GOLD);
    DrawText("[ BATTLE LOG ]", 60, 410, 18, GOLD);
    
    int yPos = 440;
    for (const auto& log : combat.getLog()) {
        DrawText(log.message.c_str(), 60, yPos, 16, log.color);
        yPos += 25;
    }
    
    // Actions
    if (!combat.isEnemyTurn()) {
        DrawText("Press [A] to attack  [I] for inventory", 300, 650, 20, RAYWHITE);
    } else {
        DrawText("Enemy is attacking...", 400, 650, 20, RED);
    }
}

void Game::DrawInventory() {
    DrawText("=== INVENTORY ===", 380, 50, 30, GOLD);
    
    int x = 100;
    int y = 150;
    int slotSize = 120;
    int spacing = 20;
    
    for (int i = 0; i < inventory.getSize(); i++) {
        Rectangle slot = {
            (float)x + (i % 6) * (slotSize + spacing),
            (float)y + (i / 6) * (slotSize + spacing),
            (float)slotSize,
            (float)slotSize
        };
        
        DrawRectangleRec(slot, DARKGRAY);
        DrawRectangleLinesEx(slot, 2, GOLD);
        
        const Item& item = inventory.getItem(i);
        DrawText(TextFormat("[%d]", i + 1), slot.x + 5, slot.y + 5, 16, GRAY);
        DrawText(item.getName().c_str(), slot.x + 10, slot.y + 40, 14, RAYWHITE);
        DrawText(TextFormat("Value: %d", item.getValue()), slot.x + 10, slot.y + 65, 12, SKYBLUE);
    }
    
    DrawText("Press number keys to use items", 300, 600, 20, GRAY);
    DrawText("Press [I] or [ESC] to close", 320, 630, 20, GRAY);
}

void Game::DrawGameOver() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
    
    DrawText("DEFEATED", 380, 250, 50, RED);
    DrawText(TextFormat("Enemies Defeated: %d", enemiesDefeated), 380, 350, 25, GRAY);
    DrawText(TextFormat("Level Reached: %d", player->getLevel()), 380, 390, 25, GRAY);
    
    DrawText("Press [R] to restart", 390, 500, 20, GREEN);
}

// ===== HELPER METHODS =====

void Game::StartGame() {
    player = new Player(playerName, selectedClass);
    currentState = GameState::EXPLORATION;
    combat.addLog("Your journey begins...", GOLD);
}

void Game::SpawnEnemy() {
    currentEnemy = Enemy::createRandomEnemy(player->getLevel());
    combat.reset();
    combat.addLog("A wild " + currentEnemy->getName() + " appears!", RED);
    currentState = GameState::COMBAT;
}

void Game::ResetGame() {
    delete player;
    delete currentEnemy;
    player = nullptr;
    currentEnemy = nullptr;
    inventory = Inventory();
    combat = Combat();
    enemiesDefeated = 0;
    nameCharCount = 0;
    memset(&playerName[0], 0, sizeof(playerName));
    currentState = GameState::MENU;
}

void Game::DrawHealthBar(Rectangle bounds, int current, int max, Color color) {
    DrawRectangleRec(bounds, DARKGRAY);
    
    float percentage = (float)current / (float)max;
    Rectangle fillBar = bounds;
    fillBar.width *= percentage;
    DrawRectangleRec(fillBar, color);
    
    DrawRectangleLinesEx(bounds, 2, BLACK);
}

void Game::DrawCharacterStats(int x, int y, Character* character) {
    DrawHealthBar({(float)x, (float)y, 250, 25}, 
                  character->getCurrentHp(), 
                  character->getMaxHp(), 
                  RED);
    DrawText(TextFormat("HP: %d/%d", character->getCurrentHp(), character->getMaxHp()), 
             x + 5, y + 5, 16, WHITE);
    
    DrawText(TextFormat("ATK: %d", character->getAttack()), x, y + 40, 18, ORANGE);
    DrawText(TextFormat("DEF: %d", character->getDefense()), x + 150, y + 40, 18, BLUE);
}
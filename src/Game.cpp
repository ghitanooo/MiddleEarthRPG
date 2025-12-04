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

    // LOAD CLASS ICONS ONCE
    wizardIcon = LoadTexture("assets/icons/wizard.png");
    hobbitIcon = LoadTexture("assets/icons/hobbit.png");
    dwarfIcon  = LoadTexture("assets/icons/dwarf.png");
    elfIcon    = LoadTexture("assets/icons/elf.png");
    rangerIcon = LoadTexture("assets/icons/ranger.png");
    // LOAD STAT ICONS
    attackIcon = LoadTexture("assets/icons/attack.png");
    defenseIcon = LoadTexture("assets/icons/defense.png");
    hpIcon = LoadTexture("assets/icons/hp.png");
    xpIcon = LoadTexture("assets/icons/xp.png");
    killsIcon = LoadTexture("assets/icons/kills.png");

}


Game::~Game() {
    delete player;
    delete currentEnemy;
    UnloadTexture(wizardIcon);
    UnloadTexture(hobbitIcon);
    UnloadTexture(dwarfIcon);
    UnloadTexture(elfIcon);
    UnloadTexture(rangerIcon);
    UnloadTexture(attackIcon);
    UnloadTexture(defenseIcon);
    UnloadTexture(hpIcon);
    UnloadTexture(xpIcon);
    UnloadTexture(killsIcon);


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
        case GameState::VICTORY:
            UpdateVictory();
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
        case GameState::VICTORY:
            DrawVictory();
            break;
        }
    
    if (showLevelUpMessage) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        const char* msg = "*** LEVEL UP! ***";
        int textWidth = MeasureText(msg, 60);
        DrawText(msg, GetScreenWidth()/2 - textWidth/2, GetScreenHeight()/2 - 30, 60, GOLD);
    }
}


void Game::UpdateMenu() {
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
    // Enemys turn 
    if (combat.isEnemyTurn()) {
        combat.updateTimer(GetFrameTime());
        
        if (combat.enemyAttackTimer >= combat.enemyAttackDelay) {
            combat.enemyAttack(*player, *currentEnemy);
            
            if (!player->isAlive()) {
                currentState = GameState::GAME_OVER;
                return;
            }
        }
        return;
    }

    // Players turn
    if (IsKeyPressed(KEY_A)) {
        combat.playerAttack(*player, *currentEnemy);
        
        if (!currentEnemy->isAlive()) {
            // Enemy died
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

            if (enemiesDefeated % 3 == 0) {  //  3 enemies = victory
                 currentState = GameState::VICTORY;
                } else {
                    currentState = GameState::EXPLORATION;
                    }
                return;
            }
    }
    
    if (IsKeyPressed(KEY_I)) {
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
void Game::UpdateVictory() {
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }
    if (IsKeyPressed(KEY_B)) {
        SpawnEnemy();
        currentState = GameState::COMBAT;
    }
}

void Game::DrawVictory() {
    ClearBackground(Color{245, 235, 220, 255});
    
    DrawText("=== VICTORY ===", 350, 200, 50, Color{34, 139, 34, 255});
    DrawText(TextFormat("%s triumphs!", player->getName().c_str()), 300, 280, 30, Color{139, 69, 19, 255});
    DrawText(TextFormat("Level: %d", player->getLevel()), 400, 340, 25, Color{85, 107, 47, 255});
    DrawText(TextFormat("Enemies Defeated: %d", enemiesDefeated), 330, 380, 25, Color{85, 107, 47, 255});
    
    DrawText("Press [B] for next battle", 350, 480, 20, Color{107, 142, 35, 255});
    DrawText("Press [R] to restart", 380, 520, 20, Color{160, 82, 45, 255});
}

//DRAAAAAAAAAAAAAAAAAAAAAWWWWWWWWWWWWWWWWWWWWWW METHODS
void Game::DrawMenu() {
    ClearBackground(Color{245, 235, 220, 255}); 
    
    DrawText("JOURNEY THROUGH", 280, 80, 45, Color{139, 69, 19, 255});
    DrawText("MIDDLE-EARTH", 310, 135, 45, Color{85, 107, 47, 255});
    DrawText("~ A Mini RPG Adventure ~", 340, 190, 22, Color{160, 82, 45, 255});
    
    DrawRectangle(300, 270, 424, 70, Color{222, 213, 195, 255});
    DrawRectangleLines(300, 270, 424, 70, Color{139, 69, 19, 255});
    DrawText("Enter thy name, traveler:", 320, 240, 22, Color{85, 107, 47, 255});
    DrawText(playerName, 320, 290, 28, Color{34, 139, 34, 255});
    
    if (nameCharCount > 0) {
        DrawText("Press ENTER to begin thy quest", 330, 380, 22, Color{34, 139, 34, 255});
    } else {
        DrawText("Speak thy name...", 390, 380, 20, Color{160, 82, 45, 200});
    }
}

void Game::DrawClassSelect() {
    ClearBackground(Color{245, 235, 220, 255});
    
DrawText("CHOOSE THY PATH", 320, 60, 40, Color{139, 69, 19, 255});

const char* classes[] = {
    "[1] WIZARD", 
    "[2] HOBBIT", 
    "[3] DWARF", 
    "[4] ELF", 
    "[5] RANGER"
};

const char* stats[] = {
    "HP:70  ATK:18 DEF:2  - Glass Cannon",
    "HP:90  ATK:12 DEF:6  - Sturdy & Brave",
    "HP:110 ATK:14 DEF:8  - Mountain Strong",
    "HP:80  ATK:16 DEF:4  - Swift & Deadly",
    "HP:85  ATK:15 DEF:5  - Jack of All Trades"
};

int startY = 140;
int spacing = 85;

for (int i = 0; i < 5; i++) {
    bool isSelected = (selectedClass == static_cast<CharacterClass>(i));

    Color boxColor  = isSelected ? Color{34, 139, 34, 80} : Color{222, 213, 195, 255};
    Color textColor = isSelected ? Color{34, 139, 34, 255} : Color{85, 107, 47, 255};

    DrawRectangle(200, startY + i * spacing, 624, 70, boxColor);
    DrawRectangleLines(200, startY + i * spacing, 624, 70, Color{139, 69, 19, 255});

    Texture2D icon;
    switch (i) {
        case 0: icon = wizardIcon; break;
        case 1: icon = hobbitIcon; break;
        case 2: icon = dwarfIcon;  break;
        case 3: icon = elfIcon;    break;
        case 4: icon = rangerIcon; break;
    }

    DrawTextureEx(icon, {202, (float)(startY + i * spacing + 15)}, 0.0f, 0.55f, Fade(WHITE, 1.0f));

    DrawText(classes[i], 270, startY + i * spacing + 12, 26, textColor);
    DrawText(stats[i], 270, startY + i * spacing + 38, 16, Color{160, 82, 45, 255});
}

DrawText("Press ENTER to embark", 370, 680, 22, Color{34, 139, 34, 255});

}

void Game::DrawExploration() {
    ClearBackground(Color{245, 235, 220, 255});
    
    DrawText("~ THE SHIRE ~", 380, 25, 35, Color{34, 139, 34, 255});
    
    DrawRectangle(70, 100, 350, 240, Color{222, 213, 195, 255});
    DrawRectangleLines(70, 100, 350, 240, Color{139, 69, 19, 255});
    
    DrawText(player->getName().c_str(), 90, 120, 28, Color{85, 107, 47, 255});
    DrawText(TextFormat("Lv.%d %s", player->getLevel(), player->getClassName().c_str()), 
             90, 150, 20, Color{160, 82, 45, 255});
    
    DrawHealthBar({90, 185, 320, 25}, player->getCurrentHp(), player->getMaxHp(), 
                  Color{220, 20, 60, 255});
    DrawText(TextFormat("HP: %d/%d", player->getCurrentHp(), player->getMaxHp()), 
             95, 188, 18, Color{255, 255, 255, 255});
    

    // ATK + DEF
    DrawTextureEx(attackIcon, {90, 220}, 0.0f, 0.35f, WHITE);
    DrawText(TextFormat("%d", player->getAttack()), 125, 225, 20, Color{139, 69, 19, 255});

    DrawTextureEx(defenseIcon, {180, 220}, 0.0f, 0.35f, WHITE);
    DrawText(TextFormat("%d", player->getDefense()), 215, 225, 20, Color{85, 107, 47, 255});

    // XP
    DrawTextureEx(xpIcon, {90, 250}, 0.0f, 0.35f, WHITE);
    DrawText(TextFormat("%d/%d XP", player->getExperience(), player->getXpToNextLevel()), 125, 255, 18, Color{85, 107, 47, 255});

    // Foes Vanquished
    DrawTextureEx(killsIcon, {90, 280}, 0.0f, 0.35f, WHITE);
    DrawText(TextFormat("Foes Vanquished: %d", enemiesDefeated), 125, 285, 18, Color{34, 139, 34, 255});

    
    // Action button
    DrawRectangle(520, 250, 350, 90, Color{34, 139, 34, 100});
    DrawRectangleLines(520, 250, 350, 90, Color{34, 139, 34, 255});
    DrawText("Press [B]", 630, 270, 24, Color{34, 139, 34, 255});
    DrawText("to seek adventure!", 590, 300, 20, Color{85, 107, 47, 255});
    
    DrawText("Press [I] to check thy pack", 370, 700, 20, Color{160, 82, 45, 255});
}

void Game::DrawCombat() {
    ClearBackground(Color{245, 235, 220, 255});
    
    DrawText("=== BATTLE ===", 380, 20, 35, Color{220, 20, 60, 255});
    
    // Player side 
    DrawRectangle(70, 90, 400, 270, Color{144, 238, 144, 100});
    DrawRectangleLines(70, 90, 400, 270, Color{34, 139, 34, 255});
    DrawText(player->getName().c_str(), 90, 110, 24, Color{34, 139, 34, 255});
    DrawText(player->getClassName().c_str(), 90, 138, 18, Color{85, 107, 47, 255});
    DrawCharacterStats(90, 170, player);
    
    // Enemy side 
    DrawRectangle(554, 90, 400, 270, Color{255, 182, 193, 100});
    DrawRectangleLines(554, 90, 400, 270, Color{220, 20, 60, 255});
    DrawText(currentEnemy->getName().c_str(), 574, 110, 24, Color{220, 20, 60, 255});
    DrawCharacterStats(574, 170, currentEnemy);
    
    // Battle log 
    DrawRectangle(70, 390, 884, 220, Color{250, 240, 230, 255});
    DrawRectangleLines(70, 390, 884, 220, Color{139, 69, 19, 255});
    DrawText("[ CHRONICLE OF BATTLE ]", 80, 405, 20, Color{139, 69, 19, 255});
    
    int yPos = 440;
    for (const auto& log : combat.getLog()) {
        DrawText(log.message.c_str(), 80, yPos, 17, log.color);
        yPos += 27;
    }
    
    // Actions
    if (!combat.isEnemyTurn()) {
        DrawText("Press [A] to strike    [I] for inventory", 305, 670, 22, Color{34, 139, 34, 255});
    } else {
        DrawText("The foe attacks...", 410, 670, 22, Color{220, 20, 60, 255});
    }
}

void Game::DrawInventory() {
    ClearBackground(Color{245, 235, 220, 255});
    
    DrawText("=== THY PACK ===", 390, 40, 35, Color{139, 69, 19, 255});
    
    int x = 140;
    int y = 140;
    int slotSize = 130;
    int spacing = 20;
    
    for (int i = 0; i < inventory.getSize(); i++) {
        Rectangle slot = {
            (float)x + (i % 5) * (slotSize + spacing),
            (float)y + (i / 5) * (slotSize + spacing),
            (float)slotSize,
            (float)slotSize
        };
        
        DrawRectangleRec(slot, Color{222, 213, 195, 255});
        DrawRectangleLinesEx(slot, 3, Color{139, 69, 19, 255});
        
        const Item& item = inventory.getItem(i);
        DrawText(TextFormat("[%d]", i + 1), slot.x + 8, slot.y + 8, 18, Color{160, 82, 45, 255});
        DrawText(item.getName().c_str(), slot.x + 12, slot.y + 50, 16, Color{34, 139, 34, 255});
        DrawText(TextFormat("+%d", item.getValue()), slot.x + 12, slot.y + 80, 15, Color{85, 107, 47, 255});
    }
    
    DrawText("Press number to use    [I]/[ESC] to close", 310, 680, 20, Color{160, 82, 45, 255});
}

void Game::DrawGameOver() {
    ClearBackground(Color{50, 40, 35, 255}); 
    
    DrawText("THOU HAST FALLEN", 310, 220, 50, Color{220, 20, 60, 255});
    DrawText(TextFormat("Foes Vanquished: %d", enemiesDefeated), 370, 330, 26, Color{245, 235, 220, 255});
    DrawText(TextFormat("Level Reached: %d", player->getLevel()), 390, 370, 26, Color{245, 235, 220, 255});
    
    DrawText("Press [R] to try again", 380, 480, 24, Color{34, 139, 34, 255});
}
void Game::DrawCharacterStats(int x, int y, Character* character) {
    DrawHealthBar({(float)x, (float)y, 280, 28}, 
                  character->getCurrentHp(), 
                  character->getMaxHp(), 
                  Color{220, 20, 60, 255});
    DrawText(TextFormat("HP: %d/%d", character->getCurrentHp(), character->getMaxHp()), 
             x + 8, y + 6, 18, Color{255, 255, 255, 255});
    
    // Draw stat icons
    DrawTextureEx(attackIcon, {(float)x, (float)(y + 45)}, 0.0f, 0.35f, WHITE);
    DrawText(TextFormat("%d", character->getAttack()), x + 40, y + 50, 20, Color{139, 69, 19, 255});

    DrawTextureEx(defenseIcon, {(float)x + 120, (float)(y + 45)}, 0.0f, 0.35f, WHITE);
    DrawText(TextFormat("%d", character->getDefense()), x + 160, y + 50, 20, Color{85, 107, 47, 255});

}

// HELPEEEEEEEEEEEEEEEEEEEER METHODS

void Game::StartGame() {
    delete player;
    player = new Player(playerName, selectedClass);
    enemiesDefeated = 0;
    currentEnemy = nullptr;
    currentState = GameState::EXPLORATION;

}

void Game::SpawnEnemy() {
    delete currentEnemy;

    int enemyType = GetRandomValue(1, 3);
    if (enemyType == 1)
        currentEnemy = new Enemy(EnemyType::GOBLIN_SCOUT);
    else if (enemyType == 2)
        currentEnemy = new Enemy(EnemyType::ORC_WARRIOR);
    else
        currentEnemy = new Enemy(EnemyType::NAZGUL);


    combat.reset();           
    combat.setPlayerTurn(true);
    currentState = GameState::COMBAT;
}

void Game::ResetGame() {
    delete player;
    delete currentEnemy;
    player = nullptr;
    currentEnemy = nullptr;
    enemiesDefeated = 0;
    showLevelUpMessage = false;
    levelUpTimer = 0.0f;
    nameCharCount = 0;
    playerName[0] = '\0';
    currentState = GameState::MENU;
}

void Game::DrawHealthBar(Rectangle bounds, int current, int max, Color color) {
    DrawRectangleLinesEx(bounds, 2, BLACK);
    float filledWidth = (float)current / (float)max * bounds.width;
    DrawRectangle(bounds.x, bounds.y, filledWidth, bounds.height, color);
}

#pragma once
#include "Character.h"
#include "GameState.h"

class Player : public Character {
private:
    CharacterClass playerClass;
    int level;
    int experience;
    int xpToNextLevel;
    
public:
    Player(std::string n, CharacterClass cls);
    
    CharacterClass getClass() const { return playerClass; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    int getXpToNextLevel() const { return xpToNextLevel; }
    
    void gainExperience(int xp);
    void levelUp();
    std::string getClassName() const;
    
    static void getClassStats(CharacterClass cls, int& hp, int& atk, int& def);
};
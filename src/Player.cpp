#include "Player.h"
#include <cmath>

Player::Player(std::string n, CharacterClass cls) 
    : Character(n, 100, 20, 5), playerClass(cls), level(1), experience(0), xpToNextLevel(100) {
    
    int hp, atk, def;
    getClassStats(cls, hp, atk, def);
    
    maxHp = hp;
    currentHp = hp;
    attack = atk;
    defense = def;
}

void Player::getClassStats(CharacterClass cls, int& hp, int& atk, int& def) {
    switch(cls) {
        case CharacterClass::WIZARD:
            hp = 80; atk = 25; def = 3;
            break;
        case CharacterClass::HOBBIT:
            hp = 100; atk = 15; def = 8;
            break;
        case CharacterClass::DWARF:
            hp = 120; atk = 20; def = 10;
            break;
        case CharacterClass::ELF:
            hp = 90; atk = 22; def = 5;
            break;
        case CharacterClass::RANGER:
            hp = 110; atk = 18; def = 7;
            break;
    }
}

std::string Player::getClassName() const {
    switch(playerClass) {
        case CharacterClass::WIZARD: return "Wizard";
        case CharacterClass::HOBBIT: return "Hobbit";
        case CharacterClass::DWARF: return "Dwarf";
        case CharacterClass::ELF: return "Elf";
        case CharacterClass::RANGER: return "Ranger";
        default: return "Unknown";
    }
}

void Player::gainExperience(int xp) {
    experience += xp;
    while (experience >= xpToNextLevel) {
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    experience -= xpToNextLevel;
    xpToNextLevel = static_cast<int>(xpToNextLevel * 1.5f);
    
    maxHp += 20;
    currentHp += 20;
    attack += 5;
    defense += 2;
}
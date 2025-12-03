#pragma once

enum class GameState {
    MENU,
    CLASS_SELECT,
    EXPLORATION,
    COMBAT,
    INVENTORY,
    GAME_OVER
};

enum class CharacterClass {
    WIZARD,
    HOBBIT,
    DWARF,
    ELF,
    RANGER
};
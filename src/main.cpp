#include "raylib.h"
#include "Game.h"

int main() {
    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    InitWindow(screenWidth, screenHeight, "Journey Through Middle-earth");
    SetTargetFPS(60);
    
    Game game;
    
    while (!WindowShouldClose()) {
        game.Update();
        
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
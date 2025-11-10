#include "jogo.h"
#include "raylib.h"

Character policia; 
Character ladrao;

Texture2D texturaladrao;
Texture2D texturapolicia;

GameScreen currentScreen = GAMEPLAY; 
int gameResult = 0;                  
float gameTimer = 0.0f;              

int main(void){
    const int larguraTela = MAP_COLS * TILE_SIZE;
    const int alturaTela = MAP_ROWS * TILE_SIZE;

    InitWindow(larguraTela, alturaTela, "Policia vs Ladrao"); 
    
    texturaladrao = LoadTexture("textureladrao.png"); 
    texturapolicia = LoadTexture("texturepolicia.png"); 
    
    SetTargetFPS(60); 

    InitGame(); 
    
    while (!WindowShouldClose()){ 
        float dt = GetFrameTime(); 
        
        UpdateGame(dt); 
        DrawGame(); 
    }
    
    UnloadTexture(texturaladrao);
    UnloadTexture(texturapolicia);
    CloseWindow(); 
    return 0;
}
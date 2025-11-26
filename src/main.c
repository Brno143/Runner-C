#include "jogo.h"
#include "raylib.h"
#include <stdio.h>

int lastScore = 0;
char playerName[31] = "";
int enteringName = 0;

int TILE_SIZE = 30;

Character policial;
Character ladrao;

int numItems = 0;
int itemsCollected = 0;

Texture2D texturaladrao;
Texture2D texturapolicia;
Texture2D texturawall;
Texture2D texturakey = {0};
Texture2D texturapower_boost = {0}; 
Texture2D texturapower_stun = {0};
Texture2D texturapower_trap = {0};

GameScreen currentScreen = MENU;
int gameResult = 0;
float gameTimer = 0.0f;              

int main(void){
    const int larguraTela = 1920;
    const int alturaTela = 1080;

    InitWindow(larguraTela, alturaTela, "RUNNER-C");
    SetWindowPosition(0, 0);

    int tileSizeW = larguraTela / MAP_COLS;
    int tileSizeH = alturaTela / MAP_ROWS;
    TILE_SIZE = (tileSizeW < tileSizeH) ? tileSizeW : tileSizeH;
    
    texturaladrao = LoadTexture("textureladrao.png"); 
    texturapolicia = LoadTexture("texturepolicia.png"); 
    texturawall = LoadTexture("texturewall.png");
    texturakey = LoadTexture("texture_key.png");
    
    texturapower_boost = LoadTexture("power_boost.png");
    texturapower_stun = LoadTexture("power_stun.png");
    texturapower_trap = LoadTexture("power_trap.png");
    
    printf("DEBUG Texturas:\n");
    printf("  Ladrao: %dx%d (id=%d)\n", texturaladrao.width, texturaladrao.height, texturaladrao.id);
    printf("  Policia: %dx%d (id=%d)\n", texturapolicia.width, texturapolicia.height, texturapolicia.id);
    printf("  Wall: %dx%d (id=%d)\n", texturawall.width, texturawall.height, texturawall.id);
    printf("  Power Boost: %dx%d (id=%d)\n", texturapower_boost.width, texturapower_boost.height, texturapower_boost.id);
    printf("  Power Stun: %dx%d (id=%d)\n", texturapower_stun.width, texturapower_stun.height, texturapower_stun.id);
    printf("  Power Trap: %dx%d (id=%d)\n", texturapower_trap.width, texturapower_trap.height, texturapower_trap.id);
    
    SetTargetFPS(60);

    Ranking_LoadPolicia();
    Ranking_LoadLadrao();

    InitGame();
    
    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        
        UpdateGame(dt);
        DrawGame();
    }
    
    UnloadTexture(texturaladrao);
    UnloadTexture(texturapolicia);
    UnloadTexture(texturawall);
    UnloadTexture(texturakey);
    UnloadTexture(texturapower_boost);
    UnloadTexture(texturapower_stun);
    UnloadTexture(texturapower_trap);
    CloseWindow();
    return 0;
}
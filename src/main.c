#include "jogo.h"
#include "raylib.h"

// Variáveis definidas externamente em jogo.h
int lastScore = 0;
char playerName[32] = "";
int enteringName = 0;

Character policias[MAX_POLICIAS];
int numPolicias = 1; // Padrão: 1 policial (modo clássico)
Character ladrao;

Texture2D texturaladrao;
Texture2D texturapolicia;

GameScreen currentScreen = MENU; // Começa no menu
int gameResult = 0;
int winnerPoliceIndex = -1; // -1 = nenhum
float gameTimer = 0.0f;              

int main(void){
    const int larguraTela = MAP_COLS * TILE_SIZE;
    const int alturaTela = MAP_ROWS * TILE_SIZE;

    InitWindow(larguraTela, alturaTela, "Policia vs Ladrao"); 
    
    texturaladrao = LoadTexture("textureladrao.png"); 
    texturapolicia = LoadTexture("texturepolicia.png"); 
    
    SetTargetFPS(60); 

    // carrega ambos rankings dos arquivos (se existirem)
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
    CloseWindow(); 
    return 0;
}
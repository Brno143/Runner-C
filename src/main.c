#include "jogo.h"
#include "raylib.h"
#include <stdio.h>

// Variáveis definidas externamente em jogo.h
int lastScore = 0;
char playerName[31] = "";
int enteringName = 0;

// Runtime tile size (defined here so other modules can use it via extern in jogo.h)
int TILE_SIZE = 30; // logical tile size (pixels) used by physics
float SCALE_X = 1.0f; // drawing scale (screen/logical)
float SCALE_Y = 1.0f;

Character policial;
Character ladrao;

Item items[MAX_ITEMS];
int numItems = 0;
int itemsCollected = 0;

Texture2D texturaladrao;
Texture2D texturapolicia;
Texture2D texturawall;
Texture2D texturakey = {0};
Texture2D texturapower_boost = {0}; 
Texture2D texturapower_stun = {0};
Texture2D texturapower_trap = {0};

GameScreen currentScreen = MENU; // Começa no menu
int gameResult = 0;
int winnerPoliceIndex = -1; // -1 = nenhum
float gameTimer = 0.0f;              

int main(void){
    // Define a resolução da janela
    const int larguraTela = 1920;  // Largura em pixels
    const int alturaTela = 1080;   // Altura em pixels

    InitWindow(larguraTela, alturaTela, "Policia vs Ladrao");
    // place window at top-left of the monitor
    SetWindowPosition(0, 0);

    // Calcula o tamanho do tile para preencher toda a tela
    // Usa o MENOR valor para garantir que o mapa caiba completamente
    // As variáveis monitorW e monitorH foram removidas daqui, eliminando os warnings.
    int tileSizeW = larguraTela / MAP_COLS;
    int tileSizeH = alturaTela / MAP_ROWS;
    TILE_SIZE = (tileSizeW < tileSizeH) ? tileSizeW : tileSizeH;
    
    // Não precisa mais de escala, pois o TILE_SIZE já foi ajustado
    SCALE_X = 1.0f;
    SCALE_Y = 1.0f;
    
    texturaladrao = LoadTexture("textureladrao.png"); 
    texturapolicia = LoadTexture("texturepolicia.png"); 
    texturawall = LoadTexture("texturewall.png");
    texturakey = LoadTexture("texture_key.png");
    
    // Carrega texturas dos power-ups (adicione os arquivos na raiz do projeto)
    texturapower_boost = LoadTexture("power_boost.png");
    texturapower_stun = LoadTexture("power_stun.png");
    texturapower_trap = LoadTexture("power_trap.png");
    
    // Debug: Verifica se as texturas foram carregadas
    printf("DEBUG Texturas:\n");
    printf("  Ladrao: %dx%d (id=%d)\n", texturaladrao.width, texturaladrao.height, texturaladrao.id);
    printf("  Policia: %dx%d (id=%d)\n", texturapolicia.width, texturapolicia.height, texturapolicia.id);
    printf("  Wall: %dx%d (id=%d)\n", texturawall.width, texturawall.height, texturawall.id);
    printf("  Power Boost: %dx%d (id=%d)\n", texturapower_boost.width, texturapower_boost.height, texturapower_boost.id);
    printf("  Power Stun: %dx%d (id=%d)\n", texturapower_stun.width, texturapower_stun.height, texturapower_stun.id);
    printf("  Power Trap: %dx%d (id=%d)\n", texturapower_trap.width, texturapower_trap.height, texturapower_trap.id);
    
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
    UnloadTexture(texturawall);
    UnloadTexture(texturakey);
    UnloadTexture(texturapower_boost);
    UnloadTexture(texturapower_stun);
    UnloadTexture(texturapower_trap);
    CloseWindow(); 
    return 0;
}
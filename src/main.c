#include "jogo.h"
#include "raylib.h"

// Variáveis definidas externamente em jogo.h
int lastScore = 0;
char playerName[31] = "";
int enteringName = 0;

// Runtime tile size (defined here so other modules can use it via extern in jogo.h)
int TILE_SIZE = 30; // logical tile size (pixels) used by physics
float SCALE_X = 1.0f; // drawing scale (screen/logical)
float SCALE_Y = 1.0f;

Character policias[MAX_POLICIAS];
int numPolicias = 1; // Padrão: 1 policial (modo clássico)
Character ladrao;

Item items[MAX_ITEMS];
int numItems = 0;
int itemsCollected = 0;

Texture2D texturaladrao;
Texture2D texturapolicia;
Texture2D texturapower_boost; 
Texture2D texturapower_stun;
Texture2D texturapower_trap;

GameScreen currentScreen = MENU; // Começa no menu
int gameResult = 0;
int winnerPoliceIndex = -1; // -1 = nenhum
float gameTimer = 0.0f;              

int main(void){
    // Create a window that matches the monitor resolution and compute drawing scale
    int monitorW = GetMonitorWidth(0);
    int monitorH = GetMonitorHeight(0);

    const int larguraTela = 1920;  // Largura em pixels
    const int alturaTela = 1080;   // Altura em pixels

    InitWindow(larguraTela, alturaTela, "Policia vs Ladrao");
    // place window at top-left of the monitor
    SetWindowPosition(0, 0);

    // Calcula o tamanho do tile para preencher toda a tela
    // Usa o MENOR valor para garantir que o mapa caiba completamente
    int tileSizeW = larguraTela / MAP_COLS;
    int tileSizeH = alturaTela / MAP_ROWS;
    TILE_SIZE = (tileSizeW < tileSizeH) ? tileSizeW : tileSizeH;
    
    // Não precisa mais de escala, pois o TILE_SIZE já foi ajustado
    SCALE_X = 1.0f;
    SCALE_Y = 1.0f;
    
    texturaladrao = LoadTexture("textureladrao.png"); 
    texturapolicia = LoadTexture("texturepolicia.png"); 
    // Carrega texturas dos power-ups (adicione os arquivos na raiz do projeto)
    texturapower_boost = LoadTexture("power_boost.png");
    texturapower_stun = LoadTexture("power_stun.png");
    texturapower_trap = LoadTexture("power_trap.png");
    
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
    UnloadTexture(texturapower_boost);
    UnloadTexture(texturapower_stun);
    UnloadTexture(texturapower_trap);
    CloseWindow(); 
    return 0;
}
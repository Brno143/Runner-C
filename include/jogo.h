#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include <stddef.h>

// Variáveis de Escala/Layout
extern int TILE_SIZE; // Logical tile size
extern float SCALE_X;
extern float SCALE_Y;

#define MAP_ROWS 17 // Seu mapa tem 17 linhas
#define MAP_COLS 30 // Seu mapa tem 30 colunas
#define TILE_EMPTY 0 // 0 para espaço vazio
#define TILE_WALL 1 // 1 para parede
#define TILE_ITEM 2 // 2 para Item/Moeda
#define MAX_POLICIAS 3 
#define MAX_ITEMS 50 // Máximo razoável para os itens/moedas

// ===== RANKING SYSTEM (MANTIDO) =====
#define RANKING_SIZE 10
#define RANKING_NAME_MAX 31

typedef struct ScoreEntry {
    char name[RANKING_NAME_MAX+1];
    int score;
} ScoreEntry;

typedef struct Character {
    Vector2 position; 
    Vector2 velocity;
    float speed; 
    float radius; 
    int role;
    int active; 
    int playerIndex;
}Character;

// NOVO: Estrutura para os itens/moedas
typedef struct Item {
    Vector2 position;
    int row;
    int col;
    int collected; // 0 = Não coletado, 1 = Coletado
} Item;

typedef enum gamescreen {   
    MENU = 0,
    GAMEPLAY = 1,
    END_GAME = 2,
    
}GameScreen;

// Funções de inicialização
void InitGame(void);

// Funções de atualização do Game Loop
void UpdateGame(float dt);

// Funções de desenho
void DrawGame(void);

// ===== VARIÁVEIS EXTERNAS =====
extern int lastScore;
extern char playerName[32];
extern int enteringName;

// Variáveis de Jogo
extern Character policias[MAX_POLICIAS]; 
extern int numPolicias; 
extern Character ladrao;

extern Item items[MAX_ITEMS]; // VARIÁVEL EXTERNA PARA OS ITENS
extern int numItems;          // VARIÁVEL EXTERNA: Total de itens no mapa
extern int itemsCollected;    // VARIÁVEL EXTERNA: Itens coletados

extern Texture2D texturaladrao;
extern Texture2D texturapolicia;

extern int gameMap[MAP_ROWS][MAP_COLS];

extern GameScreen currentScreen; 
extern int gameResult;          
extern int winnerPoliceIndex;   
extern float gameTimer;         

// Funções de Ranking (assumindo que existem)
void Ranking_LoadPolicia(void);
void Ranking_LoadLadrao(void);
void Ranking_AddPolicia(const char* name, int score);
void Ranking_AddLadrao(const char* name, int score);
void Ranking_DrawPolicia(int x, int y);
void Ranking_DrawLadrao(int x, int y);


#endif
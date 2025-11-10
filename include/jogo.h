#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include <stddef.h>

#define TILE_SIZE 60
#define MAP_ROWS 15 //linhas do mapa 
#define MAP_COLS 20 //colunas do mapa 
#define TILE_EMPTY 0 //0 para espaço vazio
#define TILE_WALL 1 //1 para parede
#define MAX_POLICIAS 3 // Máximo de policiais simultâneos

// ===== RANKING SYSTEM =====
#define RANKING_SIZE 10
#define RANKING_NAME_MAX 31

typedef struct ScoreEntry {
    char name[RANKING_NAME_MAX+1];
    int score;
} ScoreEntry;


typedef struct Character {
    Vector2 position; // aramzena a posição do personagem (X e Y)
    Vector2 velocity;   //armazena a velocidade do personagem (X e Y)
    float speed; //define a velocidade maxima
    float radius; //define o tamanho do personagem
    int role;
    int active; // 1 se o jogador está ativo, 0 se não
    int playerIndex; // Índice do jogador (0, 1, 2 para policiais)
}Character;

typedef enum gamescreen {   //"struct simples " para definir os estados do jogo
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

// ===== VARIÁVEIS DE SCORE E RANKING =====
// Último placar (preenchido ao terminar a partida)
extern int lastScore;
// Buffer para o nome do jogador (entrada durante a tela de fim de jogo)
extern char playerName[32];
// Flag que indica se estamos coletando o nome (END_GAME)
extern int enteringName;

// ===== FUNÇÕES DE RANKING =====
// Carrega rankings do arquivo (ranking_policia.txt e ranking_ladrao.txt)
void Ranking_LoadPolicia(void);
void Ranking_LoadLadrao(void);
// Salva rankings atuais nos arquivos
void Ranking_SavePolicia(void);
void Ranking_SaveLadrao(void);
// Adiciona um novo score no ranking específico (mantém top RANKING_SIZE)
void Ranking_AddPolicia(const char* name, int score);
void Ranking_AddLadrao(const char* name, int score);
// Desenha os rankings na tela (usa raylib)
void Ranking_DrawPolicia(int x, int y);
void Ranking_DrawLadrao(int x, int y);

extern Character policias[MAX_POLICIAS]; // Array de policiais
extern int numPolicias; // Número de policiais ativos (1-3)
extern Character ladrao;

extern Texture2D texturaladrao;
extern Texture2D texturapolicia;

extern int gameMap[MAP_ROWS][MAP_COLS];

extern Texture2D wallTexture; 
extern Texture2D floorTexture;

extern GameScreen currentScreen; //variavel para verificar o estao atual do jogo
extern int gameResult;          // 1 se Policial vence, 0 se Ladrão vence
extern int winnerPoliceIndex;   // Qual policial capturou (0, 1, 2)
extern float gameTimer;        // cronômetro do jogo
#endif
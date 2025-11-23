#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include <stdbool.h>

// --- CONSTANTES DE TELA ---
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// --- ENUMS E CONSTANTES DE JOGO ---
#define MAP_ROWS 17
#define MAP_COLS 30
#define TILE_WALL 1
#define TILE_ITEM 2

typedef enum { MENU, GAMEPLAY, END_GAME } GameScreen;

// --- ESTRUTURAS DE PERSONAGENS ---
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float speed;
    int active;
} Character;

typedef struct {
    Vector2 position;
    int collected;
} Item;

// --- VARIÁVEIS EXTERNAS (Definidas em jogo.c, usadas em main.c e jogo.c) ---
extern GameScreen currentScreen;
extern Character ladrao;
extern Character policial;
extern int numItems;
extern int itemsCollected;
extern float gameTimer;
extern int gameResult; // 1 = Polícia vence, 0 = Ladrão vence
extern int lastScore;
extern char playerName[31];
extern int enteringName;

extern Texture2D texturaladrao;
extern Texture2D texturapolicia;
extern Texture2D texturawall;
extern Texture2D texturakey;
// Texturas de Power-ups
extern Texture2D texturapower_boost;
extern Texture2D texturapower_stun;
extern Texture2D texturapower_trap;

// --- NOVAS CONSTANTES E ESTRUTURAS PARA POWER-UPS ---
#define MAX_POWERUPS 5
#define POWERUP_DURATION_BOOST 5.0f // 5 segundos de velocidade
#define POWERUP_DURATION_STUN 3.0f  // 3 segundos de imobilização
#define TRAP_DURATION 10.0f         // 10 segundos para a armadilha desaparecer

// Tipos de Power-ups
#define POWERUP_BOOST 1      // Aumenta velocidade (Ambos)
#define POWERUP_STUN_BOMB 2  // Ladrão: Permite atordoar policial
#define POWERUP_TRAP 3       // Policial: Permite colocar armadilha

typedef struct {
    int type;
    Vector2 position;
} PowerUp;

// Nó para lista encadeada de PowerUps
typedef struct PowerUpNode {
    PowerUp powerup;
    struct PowerUpNode* next;
} PowerUpNode;

// Nó para lista encadeada de Items
typedef struct ItemNode {
    Item item;
    struct ItemNode* next;
} ItemNode;

typedef struct {
    Vector2 position;
    float timeRemaining;
} PlacedTrap;

// Nó para lista encadeada de Armadilhas
typedef struct TrapNode {
    PlacedTrap trap;
    struct TrapNode* next;
} TrapNode;

// NOVAS VARIÁVEIS EXTERNAS GLOBAIS PARA POWER-UPS/ESTADOS
extern int numActivePowerups;
extern float powerupSpawnTimer;

extern TrapNode* trapsHead;
extern float boostTimer[2]; // [0] Ladrão, [1] Policial
extern int characterState[2]; // [0] Ladrão, [1] Policial
// Estados: 0=Normal; 1=Atordoado (STUNNED - efeito); 2=Power Stun Pronto (Ladrão)

// --- NOVAS CONSTANTES E ESTRUTURAS PARA RANKING ---
#define RANKING_SIZE 10
#define RANKING_NAME_MAX 30

typedef struct {
    char name[RANKING_NAME_MAX + 1];
    int score;
} ScoreEntry;


// --- FUNÇÕES DE JOGO (Mantidas) ---
void InitGame(void);
void UpdateGame(float dt);
void DrawGame(void);

// Funções de gerenciamento de listas encadeadas
PowerUpNode* PowerUp_Create(int type, Vector2 position);
void PowerUp_AddToList(PowerUpNode** head, PowerUpNode* node);
void PowerUp_RemoveFromList(PowerUpNode** head, PowerUpNode* node);
void PowerUp_FreeList(PowerUpNode** head);

ItemNode* Item_Create(Vector2 position);
void Item_AddToList(ItemNode** head, ItemNode* node);
void Item_FreeList(ItemNode** head);

TrapNode* Trap_Create(Vector2 position);
void Trap_AddToList(TrapNode** head, TrapNode* node);
void Trap_RemoveFromList(TrapNode** head, TrapNode* node);
void Trap_FreeList(TrapNode** head);

// Funções de Ranking (CORRIGIDAS E COMPLETAS)
void Ranking_LoadPolicia(void);
void Ranking_SavePolicia(void);
void Ranking_AddPolicia(const char* name, int score);
void Ranking_DrawPolicia(int x, int y);

void Ranking_LoadLadrao(void);
void Ranking_SaveLadrao(void);
void Ranking_AddLadrao(const char* name, int score);
void Ranking_DrawLadrao(int x, int y);

#endif
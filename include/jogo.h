#ifndef JOGO_H
#define JOGO_H

#include "raylib.h"
#include <stdbool.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define MAP_ROWS 17
#define MAP_COLS 30
#define TILE_WALL 1
#define TILE_ITEM 2

typedef enum { MENU, GAMEPLAY, END_GAME } GameScreen;

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

extern GameScreen currentScreen;
extern Character ladrao;
extern Character policial;
extern int numItems;
extern int itemsCollected;
extern float gameTimer;
extern int gameResult; 
extern int lastScore;
extern char playerName[31];
extern int enteringName;

extern Texture2D texturaladrao;
extern Texture2D texturapolicia;
extern Texture2D texturawall;
extern Texture2D texturakey;
extern Texture2D texturapower_boost;
extern Texture2D texturapower_stun;
extern Texture2D texturapower_trap;

#define MAX_POWERUPS 5
#define POWERUP_DURATION_BOOST 5.0f 
#define POWERUP_DURATION_STUN 3.0f  
#define TRAP_DURATION 10.0f         

#define POWERUP_BOOST 1      
#define POWERUP_STUN_BOMB 2  
#define POWERUP_TRAP 3       

typedef struct {
    int type;
    Vector2 position;
} PowerUp;

typedef struct PowerUpNode {
    PowerUp powerup;
    struct PowerUpNode* next;
} PowerUpNode;

typedef struct ItemNode {
    Item item;
    struct ItemNode* next;
} ItemNode;

typedef struct {
    Vector2 position;
    float timeRemaining;
} PlacedTrap;

typedef struct TrapNode {
    PlacedTrap trap;
    struct TrapNode* next;
} TrapNode;

extern int numActivePowerups;
extern float powerupSpawnTimer;

extern TrapNode* trapsHead;
extern float boostTimer[2]; 
extern int characterState[2]; 
#define RANKING_SIZE 10
#define RANKING_NAME_MAX 30

typedef struct {
    char name[RANKING_NAME_MAX + 1];
    int score;
} ScoreEntry;

void InitGame(void);
void UpdateGame(float dt);
void DrawGame(void);

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

void Ranking_LoadPolicia(void);
void Ranking_SavePolicia(void);
void Ranking_AddPolicia(const char* name, int score);
void Ranking_DrawPolicia(int x, int y);

void Ranking_LoadLadrao(void);
void Ranking_SaveLadrao(void);
void Ranking_AddLadrao(const char* name, int score);
void Ranking_DrawLadrao(int x, int y);

#endif
#ifndef JOGO_H
#include "raylib.h"


#define JOGO_H
#define TILE_SIZE 60
#define MAP_ROWS 15 //linhas do mapa 
#define MAP_COLS 20 //colunas do mapa 
#define TILE_EMPTY 0 //0 para espaço vazio
#define TILE_WALL 1 //1 para parede


typedef struct Character {
    Vector2 position; // aramzena a posição do personagem (X e Y)
    Vector2 velocity;   //armazena a velocidade do personagem (X e Y)
    float speed; //define a velocidade maxima
    float radius; //define o tamanho do personagem
    int role;
}Character;

typedef enum gamescreen {   //"struct simples " para definir os estados do jogo
    GAMEPLAY =1,
    END_GAME = 2,
    
}GameScreen;
// Funções de inicialização
void InitGame(void);

// Funções de atualização do Game Loop
void UpdateGame(float dt);

// Funções de desenho
void DrawGame(void);
extern Character policia;
extern Character ladrao;

extern Texture2D texturaladrao;
extern Texture2D texturapolicia;

extern int gameMap[MAP_ROWS][MAP_COLS];

extern Texture2D wallTexture; 
extern Texture2D floorTexture;

extern GameScreen currentScreen; //variavel para verificar o estao atual do jogo
extern int gameResult;          // 1 se Policial vence, 0 se Ladrão vence (novo!)
extern float gameTimer;        // cronômetro do jogo
#endif
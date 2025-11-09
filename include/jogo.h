#ifndef JOGO_H
#define JOGO_H
#include "raylib.h"

typedef struct Character {
    Vector2 position; // aramzena a posição do personagem (X e Y)
    Vector2 velocity;   //armazena a velocidade do personagem (X e Y)
    float speed; //define a velocidade maxima
    float radius; //define o tamanho do personagem
    int role;
}Character;
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
#endif
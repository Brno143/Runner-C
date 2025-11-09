// src/main.c (CORREÇÃO)
#include "jogo.h"
const int alturaTela = 900;
const int larguraTela = 1200;

Character policia; 
Character ladrao;

Texture2D texturaladrao;
Texture2D texturapolicia;

int main(void){
    
    InitWindow(larguraTela, alturaTela, "Runner C"); //inicia a janela mostrando o titulo
    texturaladrao = LoadTexture("textureladrao.png");
    texturapolicia = LoadTexture("texturepolicia.png");
    SetTargetFPS(60); //define o fps

    InitGame(); 
    while (!WindowShouldClose()){ 
        float dt = GetFrameTime(); 
        
        UpdateGame(dt); 
        DrawGame(); // 
    }
    UnloadTexture(texturaladrao);
    UnloadTexture(texturapolicia);
    CloseWindow(); 
    return 0;
}
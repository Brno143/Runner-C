// src/main.c (CORREÇÃO)
#include "jogo.h"
const int alturaTela = 600;
const int larguraTela = 800;

Character policia; // Tipo Character com 'C' maiúsculo (conforme struct)
Character ladrao;

int main(void){
    
    InitWindow(larguraTela, alturaTela, "Runner C"); //inicia a janela mostrando o titulo
    SetTargetFPS(60); //define o fps

    InitGame(); 
    while (!WindowShouldClose()){ 
        float dt = GetFrameTime(); 
        
        UpdateGame(dt); 
        DrawGame(); // 
    }
    CloseWindow(); 
    return 0;
}
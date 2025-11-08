// src/jogo.c (CORREÇÃO)
#include "jogo.h"
#include <stdio.h>

void InitGame(void){
    policia.position = (Vector2){100,100}; //define a posição inicial da policia
    ladrao.position = (Vector2){700,350}; //define a posição incial do ladrão
    printf("jogo iniciado!\n");
}
void UpdateGame(float dt){
}

void DrawGame(void){
    BeginDrawing();
        ClearBackground(DARKGRAY); //limpa a tela substituindo com a cor cinza
        
        DrawCircleV(ladrao.position, 15, RED); //define o desenho do ladrao como um ponto vermelho
        DrawCircleV(policia.position, 15, BLUE); //define o desenho da policia como um ponto azul

        DrawText("CRONÔMETRO: 180s", 10, 10, 20, RAYWHITE); //cria um cronometro 
        
    EndDrawing();
}
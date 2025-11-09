// src/jogo.c (CORREÇÃO)
#include "jogo.h"
#include <stdio.h>
#include <raymath.h>

#define radius 15 //define o raio dos personagens

void InitGame(void){
    policia.position = (Vector2){100,100}; //define a posição inicial da policia
    ladrao.position = (Vector2){700,350}; //define a posição incial do ladrão
    
    policia.speed = 150.0f; //define a velocidade da policia
    ladrao.speed = 180.0f; //define a velocidade do ladrão
    printf("jogo iniciado!\n");
}
void UpdateGame(float dt){
    ladrao.velocity = (Vector2){0.0f, 0.0f}; //zera a velocidade do ladrão

    if(IsKeyDown(KEY_W)) ladrao.velocity.y = -1.0f; //move para cima
    if(IsKeyDown(KEY_S)) ladrao.velocity.y = 1.0f; //move para baixo
    if(IsKeyDown(KEY_A)) ladrao.velocity.x = -1.0f; //move para esquerda
    if(IsKeyDown(KEY_D)) ladrao.velocity.x = 1.0f; //move para direita

    //normaliza a velocidade para evitar movimento mais rápido na diagonal
    if ((ladrao.velocity.x != 0.0f) || (ladrao.velocity.y != 0.0f)){
        ladrao.velocity = Vector2Normalize(ladrao.velocity);
    }
    //Aplica o Movimento do Ladrão (Posição = Posição + Velocidade * Velocidade_Max * dt)
    ladrao.position.x += ladrao.velocity.x * ladrao.speed * dt;
    ladrao.position.y += ladrao.velocity.y * ladrao.speed * dt;
    //Mantém o Ladrão dentro da tela
    ladrao.position.x = Clamp(ladrao.position.x, radius, GetScreenWidth() - radius);
    ladrao.position.y = Clamp(ladrao.position.y, radius, GetScreenHeight() - radius);

    policia.velocity = (Vector2){0.0f, 0.0f};
    if (IsKeyDown(KEY_UP)) { // CIMA
        policia.velocity.y -= 1.0f; 
    }
    if (IsKeyDown(KEY_DOWN)) { // BAIXO
        policia.velocity.y += 1.0f;
    }
    if (IsKeyDown(KEY_LEFT)) { // ESQUERDA
        policia.velocity.x -= 1.0f;
    }
    if (IsKeyDown(KEY_RIGHT)) { // DIREITA
        policia.velocity.x += 1.0f;
    }
    
    //Normaliza o vetor de velocidade do Policial
    if ((policia.velocity.x != 0.0f) || (policia.velocity.y != 0.0f)) {
        policia.velocity = Vector2Normalize(policia.velocity);
    }
    
    //Aplica o Movimento do Policial
    policia.position.x += policia.velocity.x * policia.speed * dt;
    policia.position.y += policia.velocity.y * policia.speed * dt;
    //Mantém o Policial dentro da tela
    policia.position.x = Clamp(policia.position.x, radius, GetScreenWidth() - radius);
    policia.position.y = Clamp(policia.position.y, radius, GetScreenHeight() - radius);

}

void DrawGame(void){
    BeginDrawing();
        ClearBackground(DARKGRAY); //limpa a tela substituindo com a cor cinza
        
        DrawCircleV(ladrao.position, 15, RED); //define o desenho do ladrao como um ponto vermelho
        DrawCircleV(policia.position, 15, BLUE); //define o desenho da policia como um ponto azul

        DrawText("CRONÔMETRO: 180s", 10, 10, 20, RAYWHITE); //cria um cronometro 
        
    EndDrawing();
}
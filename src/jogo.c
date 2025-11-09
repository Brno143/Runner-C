#include "jogo.h"
#include <stdio.h>
#include "raymath.h"
// Raio dos personagens (Diâmetro = 60 pixels)
#define radius 30 

// --- Definição do Mapa (Matriz 15x20) ---
int gameMap[MAP_ROWS][MAP_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1}, 
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1}, 
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},  
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},  
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},  
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},  
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},  
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},  
    {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},  
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},  
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},  
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},  
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void InitGame(void){
    policia.position = (Vector2){100, 100};
    ladrao.position = (Vector2){700, 350};
    
    policia.speed = 150.0f;
    ladrao.speed = 180.0f;
    printf("jogo iniciado!\n");
}

// --- MACRO AUXILIAR PARA COLISÃO DE TILEMAP ---
// Esta macro encapsula a lógica de colisão X/Y para evitar repetição excessiva
#define APPLY_COLLISION(character) \
    /* 1. MOVIMENTO E COLISÃO X */ \
    if (character.velocity.x != 0.0f) { \
        float futureX = character.position.x + character.velocity.x * character.speed * dt; \
        int checkSideX = (character.velocity.x > 0) ? (int)(futureX + radius) : (int)(futureX - radius); \
        int futureCol = checkSideX / TILE_SIZE; \
        int currentRow = (int)(character.position.y / TILE_SIZE); \
\
        if (futureCol >= 0 && futureCol < MAP_COLS && currentRow >= 0 && currentRow < MAP_ROWS && gameMap[currentRow][futureCol] != TILE_WALL) { \
            character.position.x = futureX; \
        } else { \
            if (character.velocity.x > 0) character.position.x = futureCol * TILE_SIZE - radius; \
            else if (character.velocity.x < 0) character.position.x = (futureCol + 1) * TILE_SIZE + radius; \
        } \
    } \
\
    /* 2. MOVIMENTO E COLISÃO Y */ \
    if (character.velocity.y != 0.0f) { \
        float futureY = character.position.y + character.velocity.y * character.speed * dt; \
        int checkSideY = (character.velocity.y > 0) ? (int)(futureY + radius) : (int)(futureY - radius); \
        int futureRow = checkSideY / TILE_SIZE; \
        int currentCol = (int)(character.position.x / TILE_SIZE); \
\
        if (futureRow >= 0 && futureRow < MAP_ROWS && currentCol >= 0 && currentCol < MAP_COLS && gameMap[futureRow][currentCol] != TILE_WALL) { \
            character.position.y = futureY; \
        } else { \
            if (character.velocity.y > 0) character.position.y = futureRow * TILE_SIZE - radius; \
            else if (character.velocity.y < 0) character.position.y = (futureRow + 1) * TILE_SIZE + radius; \
        } \
    } \
\
    /* 3. Colisão de Borda (Clamping) */ \
    character.position.x = Clamp(character.position.x, radius, GetScreenWidth() - radius); \
    character.position.y = Clamp(character.position.y, radius, GetScreenHeight() - radius);

// -------------------------------------------------------------------------------------

void UpdateGame(float dt){
    // --- LÓGICA DO LADRÃO (WASD) ---
    ladrao.velocity = (Vector2){0.0f, 0.0f};

    if(IsKeyDown(KEY_W)) ladrao.velocity.y -= 1.0f;
    if(IsKeyDown(KEY_S)) ladrao.velocity.y += 1.0f;
    if(IsKeyDown(KEY_A)) ladrao.velocity.x -= 1.0f;
    if(IsKeyDown(KEY_D)) ladrao.velocity.x += 1.0f;

    if ((ladrao.velocity.x != 0.0f) || (ladrao.velocity.y != 0.0f)) {
        ladrao.velocity = Vector2Normalize(ladrao.velocity);
    }
    
    APPLY_COLLISION(ladrao);

    // --- LÓGICA DO POLICIAL (SETAS) ---
    policia.velocity = (Vector2){0.0f, 0.0f};

    if (IsKeyDown(KEY_UP)) policia.velocity.y -= 1.0f; 
    if (IsKeyDown(KEY_DOWN)) policia.velocity.y += 1.0f;
    if (IsKeyDown(KEY_LEFT)) policia.velocity.x -= 1.0f;
    if (IsKeyDown(KEY_RIGHT)) policia.velocity.x += 1.0f;
    
    if ((policia.velocity.x != 0.0f) || (policia.velocity.y != 0.0f)) {
        policia.velocity = Vector2Normalize(policia.velocity);
    }
    
    APPLY_COLLISION(policia);
}

void DrawGame(void){
    BeginDrawing();
        ClearBackground(BLACK);
        
        // --- DESENHAR O MAPA (TILEMAP) ---
        for (int row = 0; row < MAP_ROWS; row++) {
            for (int col = 0; col < MAP_COLS; col++) {
                Rectangle destRec = {col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE};

                if (gameMap[row][col] == TILE_WALL) {
                    DrawRectangleRec(destRec, DARKGRAY); 
                } else {
                    DrawRectangleRec(destRec, RAYWHITE); // Chão (WHITE no seu código, troquei para RAYWHITE para clareza)
                }
            }
        }

        // --- DESENHAR O LADRÃO (Raio 30, Diâmetro 60) ---
        DrawTexturePro(
            texturaladrao, 
            (Rectangle){ 0, 0, texturaladrao.width, texturaladrao.height },
            (Rectangle){ ladrao.position.x, ladrao.position.y, 60, 60 }, // Desenho 60x60
            (Vector2){ 30, 30 }, // Origem (centro do 60x60)
            0.0f, 
            WHITE 
        );

        // --- DESENHAR O POLICIAL (Raio 30, Diâmetro 60) ---
        DrawTexturePro(
            texturapolicia, 
            (Rectangle){ 0, 0, texturapolicia.width, texturapolicia.height }, 
            (Rectangle){ policia.position.x, policia.position.y, 60, 60 },
            (Vector2){ 30, 30 }, // Origem (centro do 60x60)
            0.0f, 
            WHITE
        );

        DrawText("CRONÔMETRO: 180s", 10, 10, 20, BLACK); // Texto escuro em fundo claro
        
    EndDrawing();
}
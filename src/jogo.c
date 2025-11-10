// src/jogo.c (COMPLETO E CORRIGIDO)
#include "jogo.h"
#include <stdio.h>
#include "raymath.h"

// Raio dos personagens (Diâmetro = 60 pixels)
#define radius 30 
#define COLLISION_DISTANCE (radius * 2) // CORRIGIDO: Era #distancia_colisao(radius * 2)
#define INITIAL_TIME 180.0f // Tempo inicial para o cronômetro

// --- Definição do Mapa (Matriz 15x20) ---
// ... (Seu mapa aqui) ...
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
    
    policia.speed = 170.0f;
    ladrao.speed = 180.0f;
    gameTimer = INITIAL_TIME; // Inicializa/reseta o tempo
    gameResult = 0;           // Reseta o resultado
    printf("jogo iniciado!\n");
}


void UpdateGame(float dt){
    // 1. CHECAGEM DE ESTADO DE JOGO
    if (currentScreen == GAMEPLAY) {
        
        // --- 1.1 LÓGICA DO LADRÃO (Input e Movimento) ---
        ladrao.velocity = (Vector2){0.0f, 0.0f};
        if(IsKeyDown(KEY_W)) ladrao.velocity.y -= 1.0f;
        if(IsKeyDown(KEY_S)) ladrao.velocity.y += 1.0f;
        if(IsKeyDown(KEY_A)) ladrao.velocity.x -= 1.0f;
        if(IsKeyDown(KEY_D)) ladrao.velocity.x += 1.0f;
        if ((ladrao.velocity.x != 0.0f) || (ladrao.velocity.y != 0.0f)) {
            ladrao.velocity = Vector2Normalize(ladrao.velocity);
        }
        
        // Colisão Ladrão X
        if (ladrao.velocity.x != 0.0f) { 
            float futureX = ladrao.position.x + ladrao.velocity.x * ladrao.speed * dt;
            int checkSideX = (ladrao.velocity.x > 0) ? (int)(futureX + radius) : (int)(futureX - radius); 
            int futureCol = checkSideX / TILE_SIZE; 
            int currentRow = (int)(ladrao.position.y / TILE_SIZE); 
            if (futureCol >= 0 && futureCol < MAP_COLS && currentRow >= 0 && currentRow < MAP_ROWS && gameMap[currentRow][futureCol] != TILE_WALL) { 
                ladrao.position.x = futureX; 
            } else { 
                if (ladrao.velocity.x > 0) ladrao.position.x = futureCol * TILE_SIZE - radius; 
                else if (ladrao.velocity.x < 0) ladrao.position.x = (futureCol + 1) * TILE_SIZE + radius; 
            } 
        } 
        // Colisão Ladrão Y
        if (ladrao.velocity.y != 0.0f) { 
            float futureY = ladrao.position.y + ladrao.velocity.y * ladrao.speed * dt;
            int checkSideY = (ladrao.velocity.y > 0) ? (int)(futureY + radius) : (int)(futureY - radius); 
            int futureRow = checkSideY / TILE_SIZE; 
            int currentCol = (int)(ladrao.position.x / TILE_SIZE); 
            if (futureRow >= 0 && futureRow < MAP_ROWS && currentCol >= 0 && currentCol < MAP_COLS && gameMap[futureRow][currentCol] != TILE_WALL) { 
                ladrao.position.y = futureY; 
            } else { 
                if (ladrao.velocity.y > 0) ladrao.position.y = futureRow * TILE_SIZE - radius; 
                else if (ladrao.velocity.y < 0) ladrao.position.y = (futureRow + 1) * TILE_SIZE + radius; 
            } 
        } 
        // Colisão de Borda Ladrão
        ladrao.position.x = Clamp(ladrao.position.x, radius, GetScreenWidth() - radius); 
        ladrao.position.y = Clamp(ladrao.position.y, radius, GetScreenHeight() - radius);
        
        
        // --- 1.2 LÓGICA DO POLICIAL (Input e Movimento) ---
        policia.velocity = (Vector2){0.0f, 0.0f};
        if (IsKeyDown(KEY_UP)) policia.velocity.y -= 1.0f; 
        if (IsKeyDown(KEY_DOWN)) policia.velocity.y += 1.0f;
        if (IsKeyDown(KEY_LEFT)) policia.velocity.x -= 1.0f;
        if (IsKeyDown(KEY_RIGHT)) policia.velocity.x += 1.0f;
        if ((policia.velocity.x != 0.0f) || (policia.velocity.y != 0.0f)) {
            policia.velocity = Vector2Normalize(policia.velocity);
        }
        
        // Colisão Policial X
        if (policia.velocity.x != 0.0f) { 
            float futureX = policia.position.x + policia.velocity.x * policia.speed * dt;
            int checkSideX = (policia.velocity.x > 0) ? (int)(futureX + radius) : (int)(futureX - radius); 
            int futureCol = checkSideX / TILE_SIZE; 
            int currentRow = (int)(policia.position.y / TILE_SIZE); 
            if (futureCol >= 0 && futureCol < MAP_COLS && currentRow >= 0 && currentRow < MAP_ROWS && gameMap[currentRow][futureCol] != TILE_WALL) { 
                policia.position.x = futureX; 
            } else { 
                if (policia.velocity.x > 0) policia.position.x = futureCol * TILE_SIZE - radius; 
                else if (policia.velocity.x < 0) policia.position.x = (futureCol + 1) * TILE_SIZE + radius; 
            } 
        } 
        // Colisão Policial Y
        if (policia.velocity.y != 0.0f) { 
            float futureY = policia.position.y + policia.velocity.y * policia.speed * dt;
            int checkSideY = (policia.velocity.y > 0) ? (int)(futureY + radius) : (int)(futureY - radius); 
            int futureRow = checkSideY / TILE_SIZE; 
            int currentCol = (int)(policia.position.x / TILE_SIZE); 
            if (futureRow >= 0 && futureRow < MAP_ROWS && currentCol >= 0 && currentCol < MAP_COLS && gameMap[futureRow][currentCol] != TILE_WALL) { 
                policia.position.y = futureY; 
            } else { 
                if (policia.velocity.y > 0) policia.position.y = futureRow * TILE_SIZE - radius; 
                else if (policia.velocity.y < 0) policia.position.y = (futureRow + 1) * TILE_SIZE + radius; 
            } 
        } 
        // Colisão de Borda Policial
        policia.position.x = Clamp(policia.position.x, radius, GetScreenWidth() - radius); 
        policia.position.y = Clamp(policia.position.y, radius, GetScreenHeight() - radius);
        
        
        // --- 1.3 LÓGICA DE CAPTURA (Colisão Círculo-Círculo) ---
        float distance = Vector2Distance(ladrao.position, policia.position);

        if (distance < COLLISION_DISTANCE) {
            currentScreen = END_GAME;
            // Policial venceu (Captura)
            gameResult = 1; 
            printf("Ladrão Capturado! Fim de Jogo.\n");
            // calcula score do vencedor e inicia entrada de nome
            // policia: score proporcional à rapidez da captura
            lastScore = (int)((INITIAL_TIME - gameTimer) * 10.0f);
            // prepara buffer para entrada de nome
            if (playerName) playerName[0] = '\0';
            enteringName = 1;
        }
        
        // --- 1.4 LÓGICA DO CRONÔMETRO ---
        gameTimer -= dt; 
        
        if (gameTimer <= 0.0f) {
            gameTimer = 0.0f; 
            currentScreen = END_GAME;
            // Ladrão venceu (Sobrevivência)
            gameResult = 0; 
            printf("Tempo esgotado! Ladrão Venceu!\n");
            // calcula score do vencedor e inicia entrada de nome
            lastScore = (int)(gameTimer * 10.0f);
            if (playerName) playerName[0] = '\0';
            enteringName = 1;
        }
        
    // 2. CHECAGEM DE ESTADO DE FIM DE JOGO
    } else if (currentScreen == END_GAME) {
        // Entrada de nome enquanto estiver em END_GAME
        if (enteringName) {
            // coleta caracteres
            int key = GetCharPressed();
            while (key > 0) {
                int len = (int)strlen(playerName);
                if (key >= 32 && key <= 125 && len < 30) {
                    playerName[len] = (char)key;
                    playerName[len+1] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = (int)strlen(playerName);
                if (len > 0) playerName[len-1] = '\0';
            }
            // Enter salva o nome; ESPAÇO salva como anônimo
            if (IsKeyPressed(KEY_ENTER)) {
                // se não digitou nome, salva como ANON
                // Salva no ranking correto baseado em quem venceu
                if (gameResult == 1) {
                    Ranking_AddPolicia(playerName, lastScore);
                } else {
                    Ranking_AddLadrao(playerName, lastScore);
                }
                enteringName = 0;
            } else if (IsKeyPressed(KEY_SPACE)) {
                // Salva anônimo no ranking correto
                if (gameResult == 1) {
                    Ranking_AddPolicia(NULL, lastScore);
                } else {
                    Ranking_AddLadrao(NULL, lastScore);
                }
                enteringName = 0;
            }
        } else {
            // Reinício padrão ao pressionar ENTER
            if (IsKeyPressed(KEY_ENTER)) {
                InitGame(); 
                currentScreen = GAMEPLAY;
            }
        }
    }
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
                    DrawRectangleRec(destRec, RAYWHITE); 
                }
            }
        }

        // --- DESENHAR O LADRÃO (Raio 30, Diâmetro 60) ---
        DrawTexturePro(
            texturaladrao, 
            (Rectangle){ 0, 0, texturaladrao.width, texturaladrao.height },
            (Rectangle){ ladrao.position.x, ladrao.position.y, 60, 60 },
            (Vector2){ 30, 30 },
            0.0f, 
            WHITE 
        );

        // --- DESENHAR O POLICIAL (Raio 30, Diâmetro 60) ---
        DrawTexturePro(
            texturapolicia, 
            (Rectangle){ 0, 0, texturapolicia.width, texturapolicia.height }, 
            (Rectangle){ policia.position.x, policia.position.y, 60, 60 },
            (Vector2){ 30, 30 }, 
            0.0f, 
            WHITE
        );

        // --- INTERFACE E TELAS DE FIM DE JOGO ---
        if (currentScreen == GAMEPLAY) {
             // Desenha o cronômetro em segundos e milissegundos
            char timerText[32];
            sprintf(timerText, "CRONÔMETRO: %.1fs", gameTimer);
            DrawText(timerText, 10, 10, 20, BLACK);
            
        } else if (currentScreen == END_GAME) {
            // Desenha a tela de FIM DE JOGO
            const char* message;
            Color messageColor;

            if (gameResult == 1) { // Policial venceu
                message = "CAPTURADO! O Policial VENCEU!";
                messageColor = RED;
            } else { // Ladrão venceu (Tempo esgotado)
                message = "TEMPO ESGOTADO! O Ladrão VENCEU!";
                messageColor = GREEN;
            }

            // Escurece a tela de fundo
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

            // Desenha a mensagem principal centralizada
            int messageLength = MeasureText(message, 40);
            DrawText(message, 
                     (GetScreenWidth() - messageLength) / 2, 
                     GetScreenHeight() / 2 - 40, 
                     40, 
                     messageColor);
                     
            // Mensagem de Reinício
            const char* restartMsg = "Pressione ENTER para Reiniciar";
            int restartLength = MeasureText(restartMsg, 20);
            DrawText(restartMsg, 
                     (GetScreenWidth() - restartLength) / 2, 
                     GetScreenHeight() / 2 + 30, 
                     20, 
                     RAYWHITE);

            // Se estiver coletando nome, desenha o campo
            if (enteringName) {
                const char* instr = "Digite seu nome e pressione ENTER (ou ESPACO para anon):";
                int iw = MeasureText(instr, 18);
                DrawText(instr, (GetScreenWidth() - iw)/2, GetScreenHeight()/2 + 70, 18, RAYWHITE);
                // desenha conteúdo digitado
                DrawText(playerName, (GetScreenWidth() - MeasureText(playerName, 20))/2, GetScreenHeight()/2 + 100, 20, LIGHTGRAY);
            }

            // Desenha ambos os rankings lado a lado
            // Ranking do Policial (esquerda)
            Ranking_DrawPolicia(50, 40);
            // Ranking do Ladrão (direita)
            Ranking_DrawLadrao(GetScreenWidth() - 260, 40);
        }
        
    EndDrawing();
}
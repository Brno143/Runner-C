// src/jogo.c - Versão Multiplayer (até 3 policiais + 1 ladrão)
#include "jogo.h"
#include <stdio.h>
#include <string.h>
#include "raymath.h"

#define radius 30 
#define COLLISION_DISTANCE (radius * 2)
#define INITIAL_TIME 180.0f

// Mapa do jogo
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

// Posições iniciais para cada policial
static Vector2 policeStartPositions[MAX_POLICIAS] = {
    {100, 100},   // Policial 1 (canto superior esquerdo)
    {1100, 100},  // Policial 2 (canto superior direito)
    {100, 800}    // Policial 3 (canto inferior esquerdo)
};

void InitGame(void){
    // Inicializa ladrão
    ladrao.position = (Vector2){700, 450};
    ladrao.speed = 180.0f;
    ladrao.active = 1;
    
    // Inicializa policiais baseado em numPolicias
    for (int i = 0; i < MAX_POLICIAS; i++){
        if (i < numPolicias){
            policias[i].position = policeStartPositions[i];
            policias[i].speed = 170.0f;
            policias[i].active = 1;
            policias[i].playerIndex = i;
        } else {
            policias[i].active = 0;
        }
    }
    
    gameTimer = INITIAL_TIME;
    gameResult = 0;
    winnerPoliceIndex = -1;
    printf("Jogo iniciado com %d policial(is)!\n", numPolicias);
}

// Helper: movimenta um personagem com colisão
static void MoveCharacter(Character* ch, float dt){
    if (!ch->active) return;
    
    // Normaliza velocidade
    if ((ch->velocity.x != 0.0f) || (ch->velocity.y != 0.0f)) {
        ch->velocity = Vector2Normalize(ch->velocity);
    }
    
    // Colisão X
    if (ch->velocity.x != 0.0f) { 
        float futureX = ch->position.x + ch->velocity.x * ch->speed * dt;
        int checkSideX = (ch->velocity.x > 0) ? (int)(futureX + radius) : (int)(futureX - radius); 
        int futureCol = checkSideX / TILE_SIZE; 
        int currentRow = (int)(ch->position.y / TILE_SIZE); 
        if (futureCol >= 0 && futureCol < MAP_COLS && currentRow >= 0 && currentRow < MAP_ROWS && gameMap[currentRow][futureCol] != TILE_WALL) { 
            ch->position.x = futureX; 
        } else { 
            if (ch->velocity.x > 0) ch->position.x = futureCol * TILE_SIZE - radius; 
            else if (ch->velocity.x < 0) ch->position.x = (futureCol + 1) * TILE_SIZE + radius; 
        } 
    } 
    
    // Colisão Y
    if (ch->velocity.y != 0.0f) { 
        float futureY = ch->position.y + ch->velocity.y * ch->speed * dt;
        int checkSideY = (ch->velocity.y > 0) ? (int)(futureY + radius) : (int)(futureY - radius); 
        int futureRow = checkSideY / TILE_SIZE; 
        int currentCol = (int)(ch->position.x / TILE_SIZE); 
        if (futureRow >= 0 && futureRow < MAP_ROWS && currentCol >= 0 && currentCol < MAP_COLS && gameMap[futureRow][currentCol] != TILE_WALL) { 
            ch->position.y = futureY; 
        } else { 
            if (ch->velocity.y > 0) ch->position.y = futureRow * TILE_SIZE - radius; 
            else if (ch->velocity.y < 0) ch->position.y = (futureRow + 1) * TILE_SIZE + radius; 
        } 
    } 
    
    // Clamp bordas
    ch->position.x = Clamp(ch->position.x, radius, GetScreenWidth() - radius); 
    ch->position.y = Clamp(ch->position.y, radius, GetScreenHeight() - radius);
}

void UpdateGame(float dt){
    if (currentScreen == MENU) {
        // Menu de seleção de número de jogadores
        if (IsKeyPressed(KEY_ONE)) {
            numPolicias = 1;
            InitGame();
            currentScreen = GAMEPLAY;
        } else if (IsKeyPressed(KEY_TWO)) {
            numPolicias = 2;
            InitGame();
            currentScreen = GAMEPLAY;
        } else if (IsKeyPressed(KEY_THREE)) {
            numPolicias = 3;
            InitGame();
            currentScreen = GAMEPLAY;
        }
        
    } else if (currentScreen == GAMEPLAY) {
        
        // === LADRÃO (WASD) ===
        ladrao.velocity = (Vector2){0.0f, 0.0f};
        if(IsKeyDown(KEY_W)) ladrao.velocity.y -= 1.0f;
        if(IsKeyDown(KEY_S)) ladrao.velocity.y += 1.0f;
        if(IsKeyDown(KEY_A)) ladrao.velocity.x -= 1.0f;
        if(IsKeyDown(KEY_D)) ladrao.velocity.x += 1.0f;
        MoveCharacter(&ladrao, dt);
        
        // === POLICIAL 1 (Setas) ===
        if (policias[0].active) {
            policias[0].velocity = (Vector2){0.0f, 0.0f};
            if (IsKeyDown(KEY_UP)) policias[0].velocity.y -= 1.0f; 
            if (IsKeyDown(KEY_DOWN)) policias[0].velocity.y += 1.0f;
            if (IsKeyDown(KEY_LEFT)) policias[0].velocity.x -= 1.0f;
            if (IsKeyDown(KEY_RIGHT)) policias[0].velocity.x += 1.0f;
            MoveCharacter(&policias[0], dt);
        }
        
        // === POLICIAL 2 (IJKL) ===
        if (numPolicias >= 2 && policias[1].active) {
            policias[1].velocity = (Vector2){0.0f, 0.0f};
            if (IsKeyDown(KEY_I)) policias[1].velocity.y -= 1.0f; 
            if (IsKeyDown(KEY_K)) policias[1].velocity.y += 1.0f;
            if (IsKeyDown(KEY_J)) policias[1].velocity.x -= 1.0f;
            if (IsKeyDown(KEY_L)) policias[1].velocity.x += 1.0f;
            MoveCharacter(&policias[1], dt);
        }
        
        // === POLICIAL 3 (TFGH) ===
        if (numPolicias >= 3 && policias[2].active) {
            policias[2].velocity = (Vector2){0.0f, 0.0f};
            if (IsKeyDown(KEY_T)) policias[2].velocity.y -= 1.0f; 
            if (IsKeyDown(KEY_G)) policias[2].velocity.y += 1.0f;
            if (IsKeyDown(KEY_F)) policias[2].velocity.x -= 1.0f;
            if (IsKeyDown(KEY_H)) policias[2].velocity.x += 1.0f;
            MoveCharacter(&policias[2], dt);
        }
        
        // === DETECÇÃO DE CAPTURA ===
        for (int i = 0; i < numPolicias; i++){
            if (!policias[i].active) continue;
            
            float distance = Vector2Distance(ladrao.position, policias[i].position);
            if (distance < COLLISION_DISTANCE) {
                currentScreen = END_GAME;
                gameResult = 1;
                winnerPoliceIndex = i;
                lastScore = (int)((INITIAL_TIME - gameTimer) * 10.0f);
                if (playerName) playerName[0] = '\0';
                enteringName = 1;
                printf("Policial %d capturou o ladrão!\n", i+1);
                return;
            }
        }
        
        // === CRONÔMETRO ===
        gameTimer -= dt;
        if (gameTimer <= 0.0f) {
            gameTimer = 0.0f;
            currentScreen = END_GAME;
            gameResult = 0;
            winnerPoliceIndex = -1;
            lastScore = (int)(INITIAL_TIME * 10.0f); // Ladrão sobreviveu = score alto
            if (playerName) playerName[0] = '\0';
            enteringName = 1;
            printf("Tempo esgotado! Ladrão venceu!\n");
        }
        
    } else if (currentScreen == END_GAME) {
        // Entrada de nome
        if (enteringName) {
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
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (gameResult == 1) {
                    Ranking_AddPolicia(playerName, lastScore);
                } else {
                    Ranking_AddLadrao(playerName, lastScore);
                }
                enteringName = 0;
            } else if (IsKeyPressed(KEY_SPACE)) {
                if (gameResult == 1) {
                    Ranking_AddPolicia(NULL, lastScore);
                } else {
                    Ranking_AddLadrao(NULL, lastScore);
                }
                enteringName = 0;
            }
        } else {
            if (IsKeyPressed(KEY_ENTER)) {
                currentScreen = MENU; // Volta ao menu
            }
        }
    }
}

void DrawGame(void){
    BeginDrawing();
        ClearBackground(BLACK);
        
        if (currentScreen == MENU) {
            // === TELA DE MENU ===
            DrawText("POLICIA vs LADRAO", GetScreenWidth()/2 - 200, 150, 40, GOLD);
            DrawText("Selecione o numero de jogadores:", GetScreenWidth()/2 - 250, 300, 25, RAYWHITE);
            
            DrawText("[1] - 2 Jogadores (1 Policial vs 1 Ladrao)", GetScreenWidth()/2 - 280, 380, 22, GREEN);
            DrawText("[2] - 3 Jogadores (2 Policiais vs 1 Ladrao)", GetScreenWidth()/2 - 280, 430, 22, GREEN);
            DrawText("[3] - 4 Jogadores (3 Policiais vs 1 Ladrao)", GetScreenWidth()/2 - 280, 480, 22, GREEN);
            
            DrawText("CONTROLES:", GetScreenWidth()/2 - 100, 580, 20, YELLOW);
            DrawText("Ladrao: WASD", 100, 630, 18, RAYWHITE);
            DrawText("Policial 1: SETAS", 100, 660, 18, RAYWHITE);
            DrawText("Policial 2: IJKL", 400, 660, 18, RAYWHITE);
            DrawText("Policial 3: TFGH", 700, 660, 18, RAYWHITE);
            
        } else if (currentScreen == GAMEPLAY) {
            // === DESENHAR O MAPA ===
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

            // === DESENHAR LADRÃO ===
            DrawTexturePro(
                texturaladrao, 
                (Rectangle){ 0, 0, texturaladrao.width, texturaladrao.height },
                (Rectangle){ ladrao.position.x, ladrao.position.y, 60, 60 },
                (Vector2){ 30, 30 },
                0.0f, 
                WHITE 
            );

            // === DESENHAR POLICIAIS ===
            Color policeColors[] = {BLUE, RED, PURPLE}; // Cores diferentes
            for (int i = 0; i < numPolicias; i++){
                if (!policias[i].active) continue;
                
                DrawTexturePro(
                    texturapolicia, 
                    (Rectangle){ 0, 0, texturapolicia.width, texturapolicia.height }, 
                    (Rectangle){ policias[i].position.x, policias[i].position.y, 60, 60 },
                    (Vector2){ 30, 30 }, 
                    0.0f, 
                    policeColors[i]
                );
                
                // Número do policial
                char label[8];
                sprintf(label, "P%d", i+1);
                DrawText(label, (int)policias[i].position.x - 10, (int)policias[i].position.y - 45, 20, policeColors[i]);
            }

            // === HUD ===
            char timerText[32];
            sprintf(timerText, "TEMPO: %.1fs", gameTimer);
            DrawText(timerText, 10, 10, 20, BLACK);
            
            char playersText[64];
            sprintf(playersText, "Policiais: %d | Ladrao: 1", numPolicias);
            DrawText(playersText, 10, 35, 18, BLACK);
            
        } else if (currentScreen == END_GAME) {
            // === TELA DE FIM ===
            const char* message;
            Color messageColor;

            if (gameResult == 1) {
                char winMsg[128];
                sprintf(winMsg, "CAPTURADO! Policial %d VENCEU!", winnerPoliceIndex + 1);
                message = winMsg;
                messageColor = RED;
            } else {
                message = "TEMPO ESGOTADO! O Ladrao VENCEU!";
                messageColor = GREEN;
            }

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
            
            int messageLength = MeasureText(message, 40);
            DrawText(message, (GetScreenWidth() - messageLength) / 2, GetScreenHeight() / 2 - 40, 40, messageColor);
            
            if (!enteringName) {
                const char* restartMsg = "Pressione ENTER para voltar ao Menu";
                int restartLength = MeasureText(restartMsg, 20);
                DrawText(restartMsg, (GetScreenWidth() - restartLength) / 2, GetScreenHeight() / 2 + 30, 20, RAYWHITE);
            }

            if (enteringName) {
                const char* instr = "Digite seu nome e pressione ENTER (ou ESPACO para anon):";
                int iw = MeasureText(instr, 18);
                DrawText(instr, (GetScreenWidth() - iw)/2, GetScreenHeight()/2 + 70, 18, RAYWHITE);
                DrawText(playerName, (GetScreenWidth() - MeasureText(playerName, 20))/2, GetScreenHeight()/2 + 100, 20, LIGHTGRAY);
            }

            // Rankings
            Ranking_DrawPolicia(50, 40);
            Ranking_DrawLadrao(GetScreenWidth() - 260, 40);
        }
        
    EndDrawing();
}

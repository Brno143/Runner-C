// src/jogo.c - Versão com Trap Automática e Controles Corrigidos
#include "../include/jogo.h"
#include <stdio.h>
#include <string.h>
#include "raymath.h"
#include <stdlib.h> 

// Variáveis de Escala/Layout
extern int TILE_SIZE; // Definido em main.c

// Constantes de cor
const Color parede = { 255, 255, 255, 255 }; 
const Color ULTRA_DARKBLUE = { 80, 80, 80, 255 }; 

#define radius ((float)TILE_SIZE / 2.0f)
#define COLLISION_DISTANCE (float)TILE_SIZE * 0.9f
#define ITEM_COLLECTION_RADIUS ((float)TILE_SIZE * 0.45f) 
#define POWERUP_COLLECTION_RADIUS ((float)TILE_SIZE * 0.5f) // Raio de coleta maior para poderes
#define INITIAL_TIME 180.0f

// Mapa do jogo (mantido)
int gameMap[MAP_ROWS][MAP_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1}, 
    {1, 0, 0, 2, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2, 0, 1, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1}, 
    {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1}, 
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1}, 
    {1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1}, 
    {1, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}, 
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}, 
    {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
};

// Posições iniciais (DECLARADAS sem inicialização)
static Vector2 policeStartPositions[MAX_POLICIAS]; 

// Variáveis Globais PowerUp e Efeitos (Definidas aqui, extern em jogo.h)
PowerUp powerups[MAX_POWERUPS];
int numActivePowerups = 0;
float powerupSpawnTimer = 0.0f;
PlacedTrap currentTrap = {0};
float boostTimer[MAX_POLICIAS + 1] = {0.0f}; 
int characterState[MAX_POLICIAS + 1] = {0}; 


// Helper: movimenta um personagem com colisão
static void MoveCharacter(Character* ch, float dt){
    if (!ch->active) return;
    
    // VERIFICA SE ESTÁ IMOBILIZADO (characterState[index] == 1)
    if (ch == &ladrao && characterState[0] == 1) return;
    for (int i = 0; i < numPolicias; i++) {
        if (ch == &policias[i] && characterState[i + 1] == 1) return;
    }
    
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
            if (ch->velocity.x > 0) ch->position.x = (float)futureCol * TILE_SIZE - radius; 
            else if (ch->velocity.x < 0) ch->position.x = (float)(futureCol + 1) * TILE_SIZE + radius; 
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
            if (ch->velocity.y > 0) ch->position.y = (float)futureRow * TILE_SIZE - radius; 
            else if (ch->velocity.y < 0) ch->position.y = (float)(futureRow + 1) * TILE_SIZE + radius; 
        } 
    } 
    
    // Clamp bordas
    ch->position.x = Clamp(ch->position.x, (float)radius, (float)GetScreenWidth() - (float)radius); 
    ch->position.y = Clamp(ch->position.y, (float)radius, (float)GetScreenHeight() - (float)radius);
}

// Helper: Tenta spawnar um PowerUp em uma célula vazia
static void TrySpawnPowerUp(int type) {
    if (numActivePowerups >= MAX_POWERUPS) return;

    int freeCells[MAP_ROWS * MAP_COLS][2];
    int freeCount = 0;
    
    // 1. Encontrar células livres (não parede, não item, não armadilha ativa)
    for (int row = 1; row < MAP_ROWS - 1; row++) {
        for (int col = 1; col < MAP_COLS - 1; col++) {
            if (gameMap[row][col] != TILE_WALL && gameMap[row][col] != TILE_ITEM) {
                
                // Verifica se já existe um powerup no local
                int occupied = 0;
                for (int i = 0; i < MAX_POWERUPS; i++) {
                    if (powerups[i].active) {
                        int p_row = (int)(powerups[i].position.y / TILE_SIZE);
                        int p_col = (int)(powerups[i].position.x / TILE_SIZE);
                        if (p_row == row && p_col == col) {
                            occupied = 1;
                            break;
                        }
                    }
                }
                if (occupied) continue;

                freeCells[freeCount][0] = row;
                freeCells[freeCount][1] = col;
                freeCount++;
            }
        }
    }

    if (freeCount > 0) {
        // 2. Escolher uma célula aleatória
        int randomIndex = rand() % freeCount;
        int row = freeCells[randomIndex][0];
        int col = freeCells[randomIndex][1];
        
        // 3. Criar o PowerUp
        int index = -1;
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!powerups[i].active) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            powerups[index].active = 1;
            powerups[index].type = type;
            powerups[index].position = (Vector2){
                (float)col * TILE_SIZE + TILE_SIZE / 2.0f,
                (float)row * TILE_SIZE + TILE_SIZE / 2.0f
            };
            numActivePowerups++;
        }
    } 
}

void InitGame(void){
    
    float current_radius = (float)TILE_SIZE / 2.0f;
    
    policeStartPositions[0] = (Vector2){(float)2 * TILE_SIZE + current_radius, (float)1 * TILE_SIZE + current_radius}; // P1
    policeStartPositions[1] = (Vector2){(float)MAP_COLS * TILE_SIZE - (2 * TILE_SIZE) - current_radius, (float)1 * TILE_SIZE + current_radius}; // P2
    policeStartPositions[2] = (Vector2){(float)2 * TILE_SIZE + current_radius, (float)MAP_ROWS * TILE_SIZE - (2 * TILE_SIZE) - current_radius}; // P3

    // Inicializa ladrão: Posição central do mapa.
    ladrao.position = (Vector2){(float)MAP_COLS * TILE_SIZE / 2.0f, (float)MAP_ROWS * TILE_SIZE / 2.0f};
    ladrao.speed = 180.0f;
    ladrao.active = 1;
    
    // Inicializa policiais
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
    
    // Inicializa ITENS (mantido)
    numItems = 0;
    itemsCollected = 0;
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            if (gameMap[row][col] == TILE_ITEM) {
                if (numItems < MAX_ITEMS) {
                    items[numItems].row = row;
                    items[numItems].col = col;
                    items[numItems].position = (Vector2){
                        (float)col * TILE_SIZE + TILE_SIZE / 2.0f,
                        (float)row * TILE_SIZE + TILE_SIZE / 2.0f
                    };
                    items[numItems].collected = 0;
                    numItems++;
                }
            }
        }
    }

    // === Inicializa POWERUPS/EFEITOS ===
    powerupSpawnTimer = (float)GetRandomValue(4, 8); // INTERVALO DE POWERUP DIMINUÍDO (4-8 segundos)
    numActivePowerups = 0;
    currentTrap.active = 0;
    for (int i = 0; i <= MAX_POLICIAS; i++) {
        boostTimer[i] = 0.0f;
        characterState[i] = 0; // Estado normal
    }
    
    gameTimer = INITIAL_TIME;
    gameResult = 0;
    winnerPoliceIndex = -1;
}

void UpdateGame(float dt){
    if (currentScreen == MENU) {
        if (IsKeyPressed(KEY_ONE)) {
            numPolicias = 1;
            InitGame();
            currentScreen = GAMEPLAY;
        }
} else if (currentScreen == GAMEPLAY) {
        
        // === GESTÃO DO TEMPORIZADOR DE SPAWN DE POWERUP ===
        powerupSpawnTimer -= dt;
        if (powerupSpawnTimer <= 0.0f) {
            // Tenta spawnar um dos 3 tipos aleatoriamente
            int randomType = GetRandomValue(POWERUP_BOOST, POWERUP_TRAP); 
            TrySpawnPowerUp(randomType);
            powerupSpawnTimer = (float)GetRandomValue(4, 8); // INTERVALO DE POWERUP DIMINUÍDO (4-8 segundos)
        }

        // === MOVIMENTOS E GESTÃO DE EFEITOS ===
        
        // Ladrão (Índice 0)
        ladrao.velocity = (Vector2){0.0f, 0.0f};
        if(IsKeyDown(KEY_W)) ladrao.velocity.y -= 1.0f;
        if(IsKeyDown(KEY_S)) ladrao.velocity.y += 1.0f;
        if(IsKeyDown(KEY_A)) ladrao.velocity.x -= 1.0f;
        if(IsKeyDown(KEY_D)) ladrao.velocity.x += 1.0f;
        
        // Ajuste de velocidade e tempo de efeito (Ladrão - Índice 0)
        ladrao.speed = 180.0f;
        if (boostTimer[0] > 0.0f) {
            ladrao.speed = 280.0f; // Boost ativo
            boostTimer[0] -= dt;
        }
        // Tempo de STUN do Ladrão (se for 1)
        if (characterState[0] == 1) { 
             ladrao.speed = 0.0f;
             boostTimer[0] -= dt;
             if (boostTimer[0] <= 0.0f) characterState[0] = 0;
        }
        
        // Lógica de STUN AURA (POWERUP_STUN_BOMB)
        if (characterState[0] == 2) { // Se o ladrão tem o poder de STUN AURA ATIVO
             if (boostTimer[0] <= 0.0f) { // Se o tempo da aura acabar
                 characterState[0] = 0; // Desativa a aura
             } else {
                 // Verifica se atinge algum policial dentro do raio da aura
                 for (int i = 0; i < numPolicias; i++) {
                    float distance = Vector2Distance(ladrao.position, policias[i].position);
                    if (policias[i].active && distance < (float)TILE_SIZE * 1.5f) { // Alcance da aura
                        if (characterState[i + 1] == 0) { // Se o policial não estiver já stunado
                            characterState[i + 1] = 1; // Policial atordoado
                            boostTimer[i + 1] = POWERUP_DURATION_STUN; // Aplica o stun (3s)
                            
                            // Gasta o poder na primeira ativação de stun
                            characterState[0] = 0; 
                            boostTimer[0] = 0.0f; 
                            break; // Sai do loop após atordoar 1 policial
                        }
                    }
                 }
             }
        }

        MoveCharacter(&ladrao, dt);
        
        // Policiais (Índices 1, 2, 3 no array characterState)
        for (int i = 0; i < numPolicias; i++) {
            Character* p = &policias[i];
            
            // Ajuste de velocidade e tempo de efeito (Policiais - Índice i+1)
            p->speed = 170.0f;
            if (boostTimer[i + 1] > 0.0f) {
                // Policial pode ter BOOST ou estar STUNNED. Se estiver STUNNED (state 1), a velocidade é 0.
                if (characterState[i + 1] == 0) {
                    p->speed = 270.0f; // Boost ativo
                }
                boostTimer[i + 1] -= dt;
            } 
            
            // Tempo de STUN do Policial (se for 1)
            if (characterState[i + 1] == 1) { 
                p->speed = 0.0f; // Imobilizado
                // O timer é controlado pelo boostTimer[i+1]
                if (boostTimer[i + 1] <= 0.0f) characterState[i + 1] = 0; // Fim do stun
            }
            
            p->velocity = (Vector2){0.0f, 0.0f};
            // Controles dos Policiais (mantidos)
            if (i == 0) {
                if (IsKeyDown(KEY_UP)) p->velocity.y -= 1.0f; 
                if (IsKeyDown(KEY_DOWN)) p->velocity.y += 1.0f;
                if (IsKeyDown(KEY_LEFT)) p->velocity.x -= 1.0f;
                if (IsKeyDown(KEY_RIGHT)) p->velocity.x += 1.0f;
            } else if (i == 1) {
                if (IsKeyDown(KEY_I)) p->velocity.y -= 1.0f; 
                if (IsKeyDown(KEY_K)) p->velocity.y += 1.0f;
                if (IsKeyDown(KEY_J)) p->velocity.x -= 1.0f;
                if (IsKeyDown(KEY_L)) p->velocity.x += 1.0f;
            } else if (i == 2) {
                if (IsKeyDown(KEY_T)) p->velocity.y -= 1.0f; 
                if (IsKeyDown(KEY_G)) p->velocity.y += 1.0f;
                if (IsKeyDown(KEY_F)) p->velocity.x -= 1.0f;
                if (IsKeyDown(KEY_H)) p->velocity.x += 1.0f;
            }

            MoveCharacter(p, dt);
        }
        
        // === GESTÃO DA ARMADILHA (TRAP) ===
        if (currentTrap.active) {
            // Armadilha fica ativa até ser acionada.
            // Checa colisão do Ladrão com a Armadilha
            float distance = Vector2Distance(ladrao.position, currentTrap.position);
            if (distance < COLLISION_DISTANCE) {
                characterState[0] = 1; // Ladrão imobilizado
                boostTimer[0] = POWERUP_DURATION_STUN; // Reutiliza timer para stun (3s)
                currentTrap.active = 0; // Armadilha ativada/gasta
            }
        }

        // === COLETA DE POWERUPS ===
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (powerups[i].active) {
                int collected = 0;
                int character_index = -1; // 0=Ladrao, 1,2,3=Policiais
                
                // 1. Ladrão coleta (index 0)
                if (Vector2Distance(ladrao.position, powerups[i].position) < POWERUP_COLLECTION_RADIUS) {
                    character_index = 0;
                    collected = 1;
                }
                
                // 2. Policiais coletam (index j+1)
                for (int j = 0; j < numPolicias; j++) {
                    if (!policias[j].active) continue;
                    if (Vector2Distance(policias[j].position, powerups[i].position) < POWERUP_COLLECTION_RADIUS) {
                        character_index = j + 1;
                        collected = 1;
                        break;
                    }
                }

                if (collected) {
                    if (powerups[i].type == POWERUP_BOOST) {
                        boostTimer[character_index] = POWERUP_DURATION_BOOST; 
                    } else if (powerups[i].type == POWERUP_STUN_BOMB && character_index == 0) { 
                        // Ladrão coleta Stun Aura, ativa-o por 5 segundos
                        characterState[0] = 2; // Estado "Stun Aura Pronto/Ativo"
                        boostTimer[0] = POWERUP_DURATION_BOOST; // Usa 5s de duração (mesmo tempo do speed)
                    } else if (powerups[i].type == POWERUP_TRAP && character_index == 1) { 
                        // MUDANÇA: Ativa a trap imediatamente!
                        currentTrap.position = powerups[i].position; // POLICIAL 1: Guarda a POSIÇÃO DO POWERUP
                        currentTrap.active = 1; // Armadilha colocada no chão AGORA
                        characterState[1] = 0; // Estado normal (não precisa de estado "Trap Ready")
                    } else if (powerups[i].type == POWERUP_TRAP && character_index > 1) {
                        // Policiais 2 e 3 não coletam a TRAP (para simplificar)
                        collected = 0; 
                    } else if (powerups[i].type == POWERUP_STUN_BOMB && character_index > 0) {
                        // Policiais não coletam STUN AURA (para simplificar)
                        collected = 0;
                    }

                    if (collected) {
                         powerups[i].active = 0;
                         numActivePowerups--;
                    }
                }
            }
        }
        
        // === COLETA DE ITENS (mantido) ===
        for (int i = 0; i < numItems; i++) {
            if (!items[i].collected) {
                float distance = Vector2Distance(ladrao.position, items[i].position);
                if (distance < ITEM_COLLECTION_RADIUS) { 
                    items[i].collected = 1;
                    itemsCollected++;
                    
                    if (itemsCollected >= numItems) {
                        currentScreen = END_GAME;
                        gameResult = 0; 
                        winnerPoliceIndex = -1;
                        lastScore = numItems * 100 + (int)(gameTimer * 5.0f); 
                        playerName[0] = '\0';
                        enteringName = 1;
                        return;
                    }
                }
            }
        }
        
        // === DETECÇÃO DE CAPTURA (mantido) ===
        for (int i = 0; i < numPolicias; i++){
            if (!policias[i].active) continue;
            
            float distance = Vector2Distance(ladrao.position, policias[i].position);
            // Captura só acontece se nenhum dos dois estiver atordoado
            if (distance < COLLISION_DISTANCE && characterState[0] == 0 && characterState[i+1] == 0) {
                currentScreen = END_GAME;
                gameResult = 1;
                winnerPoliceIndex = i;
                lastScore = (int)((INITIAL_TIME - gameTimer) * 10.0f);
                playerName[0] = '\0'; 
                enteringName = 1;
                return;
            }
        }
        
        // === CRONÔMETRO (mantido) ===
        gameTimer -= dt;
        if (gameTimer <= 0.0f) {
            gameTimer = 0.0f;
            currentScreen = END_GAME;
            gameResult = 0;
            winnerPoliceIndex = -1;
            lastScore = (int)(INITIAL_TIME * 10.0f) + (itemsCollected * 50); 
            playerName[0] = '\0'; 
            enteringName = 1;
        }
        
    } else if (currentScreen == END_GAME) {
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
                currentScreen = MENU; 
            }
        }
    }
}

void DrawGame(void){
    BeginDrawing();
        ClearBackground(BLACK);
        
        if (currentScreen == MENU) {
            DrawText("POLICIA vs LADRAO", GetScreenWidth()/2 - 200, 150, 40, GOLD);
            DrawText("Selecione o numero de jogadores:", GetScreenWidth()/2 - 250, 300, 25, RAYWHITE);
            
            DrawText("[1] - 2 Jogadores (1 Policial vs 1 Ladrao)", GetScreenWidth()/2 - 280, 380, 22, GREEN);
            
            DrawText("CONTROLES:", GetScreenWidth()/2 - 100, 580, 20, YELLOW);
            
            // LINHA CORRIGIDA
            DrawText("Ladrao: WASD | Especial: STUN AURA (Ativado ao coletar o item - 5s de efeito)", 100, 630, 18, RAYWHITE);
            
            // LINHAS CORRIGIDAS/SEPARADAS
            DrawText("Policial 1: SETAS | Especial: TRAP (Ativada no chão ao coletar o item)", 100, 660, 18, RAYWHITE);
            
        } else if (currentScreen == GAMEPLAY) {
            // === DESENHAR O MAPA ===
            for (int row = 0; row < MAP_ROWS; row++) {
                for (int col = 0; col < MAP_COLS; col++) {
                    Rectangle destRec = {(float)col * TILE_SIZE, (float)row * TILE_SIZE, (float)TILE_SIZE, (float)TILE_SIZE};
                    if (gameMap[row][col] == TILE_WALL) {
                        DrawRectangleRec(destRec, ULTRA_DARKBLUE); 
                    } else {
                        DrawRectangleRec(destRec, parede); 
                    }
                }
            }

            // === DESENHAR ITENS COLETÁVEIS ===
            for (int i = 0; i < numItems; i++) {
                if (!items[i].collected) {
                    DrawCircle((int)items[i].position.x, (int)items[i].position.y, (float)TILE_SIZE * 0.15f, GOLD);
                    DrawCircle((int)items[i].position.x, (int)items[i].position.y, (float)TILE_SIZE * 0.10f, YELLOW);
                }
            }
            
            // === DESENHAR POWERUPS ===
            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (powerups[i].active) {
                    Color color = RAYWHITE;
                    char symbol = '?';
                    if (powerups[i].type == POWERUP_BOOST) { color = LIME; symbol = 'S'; } 
                    else if (powerups[i].type == POWERUP_STUN_BOMB) { color = BLUE; symbol = 'A'; } // 'A' de Aura
                    else if (powerups[i].type == POWERUP_TRAP) { color = RED; symbol = 'T'; } 
                    
                    DrawCircle((int)powerups[i].position.x, (int)powerups[i].position.y, (float)TILE_SIZE * 0.2f, color);
                    DrawText(TextFormat("%c", symbol), (int)powerups[i].position.x - 5, (int)powerups[i].position.y - 10, 20, BLACK);
                }
            }
            
            // === DESENHAR ARMADILHA COLOCADA ===
            if (currentTrap.active) {
                DrawCircleV(currentTrap.position, (float)TILE_SIZE * 0.3f, Fade(RED, 0.7f));
                DrawText("TRAP", (int)currentTrap.position.x - 20, (int)currentTrap.position.y - 10, 15, BLACK);
            }

            // === DESENHAR LADRÃO E POLICIAIS ===
            float spriteSize = (float)TILE_SIZE;
            float spriteCenter = (float)TILE_SIZE / 2.0f;
            
            // LADRÃO
            Color ladraoTint = WHITE;
            if (boostTimer[0] > 0.0f) ladraoTint = GREEN; // Boost ativo
            if (characterState[0] == 1) ladraoTint = PURPLE; // Stunned
            
            // Desenhar Aura se o poder do ladrão estiver ativo (Stun Aura)
            if (characterState[0] == 2) {
                float ringSize = spriteCenter + 5.0f + (fmodf(GetTime() * 10, 1.0f) * 10.0f);
                Color auraColor = Fade(BLUE, 0.5f);
                DrawRing(ladrao.position, spriteCenter + 5.0f, ringSize, 0, 360, 16, auraColor);
                DrawText(TextFormat("STUN AURA: %.1fs", boostTimer[0]), (int)ladrao.position.x - 60, (int)ladrao.position.y - 60, 15, BLUE);
            }

            DrawTexturePro(
                texturaladrao, 
                (Rectangle){ 0, 0, texturaladrao.width, texturaladrao.height },
                (Rectangle){ ladrao.position.x, ladrao.position.y, spriteSize, spriteSize },
                (Vector2){ spriteCenter, spriteCenter },
                0.0f, 
                ladraoTint 
            );
            
            // POLICIAIS
            Color policeColors[] = {BLUE, RED, PURPLE}; 
            for (int i = 0; i < numPolicias; i++){
                if (!policias[i].active) continue;
                
                Color pTint = policeColors[i];
                if (boostTimer[i+1] > 0.0f) pTint = GREEN;
                if (characterState[i+1] == 1) pTint = PURPLE; // Imobilizado

                DrawTexturePro(
                    texturapolicia, 
                    (Rectangle){ 0, 0, texturapolicia.width, texturapolicia.height }, 
                    (Rectangle){ policias[i].position.x, policias[i].position.y, spriteSize, spriteSize },
                    (Vector2){ spriteCenter, spriteCenter }, 
                    0.0f, 
                    pTint
                );
                
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
            
            char itemsText[64];
            sprintf(itemsText, "Itens: %d/%d", itemsCollected, numItems);
            DrawText(itemsText, 10, 60, 18, GOLD); 
            
            DrawText(TextFormat("Próximo PowerUp em: %.1fs", powerupSpawnTimer), GetScreenWidth() - 300, 10, 18, RAYWHITE);
            
        } else if (currentScreen == END_GAME) {
            const char* message;
            Color messageColor;
            char finalMessage[128] = "";
            
            if (gameResult == 1) {
                sprintf(finalMessage, "CAPTURADO! Policial %d VENCEU! Score: %d", winnerPoliceIndex + 1, lastScore);
                message = finalMessage;
                messageColor = RED;
            } else {
                 if (itemsCollected >= numItems) {
                    sprintf(finalMessage, "FUGA COMPLETA! Ladrao VENCEU COLETANDO TUDO! Score: %d", lastScore);
                    message = finalMessage;
                    messageColor = GOLD;
                } else {
                    sprintf(finalMessage, "TEMPO ESGOTADO! Ladrao VENCEU! Score: %d", lastScore);
                    message = finalMessage;
                    messageColor = GREEN;
                }
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

            Ranking_DrawPolicia(50, 40);
            Ranking_DrawLadrao(GetScreenWidth() - 260, 40);
        }
        
    EndDrawing();
}
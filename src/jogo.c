// src/jogo.c - Versão com Trap Automática e Controles Corrigidos
#include "../include/jogo.h"
#include <stdio.h>
#include <string.h>
#include "raymath.h"
#include <stdlib.h> 

// Variáveis de Escala/Layout
extern int TILE_SIZE; // Definido em main.c

// Constantes de cor
const Color chao = { 211, 211, 211, 255 }; 
const Color ULTRA_DARKBLUE = { 80, 80, 80, 255 }; 

#define radius ((float)TILE_SIZE / 2.0f)
#define COLLISION_DISTANCE (float)TILE_SIZE * 0.9f

#define COLLISION_DISTANCE_SQ (COLLISION_DISTANCE * COLLISION_DISTANCE)
#define ITEM_COLLECTION_RADIUS ((float)TILE_SIZE * 0.45f) 
#define ITEM_COLLECTION_RADIUS_SQ (ITEM_COLLECTION_RADIUS * ITEM_COLLECTION_RADIUS)
#define POWERUP_COLLECTION_RADIUS ((float)TILE_SIZE * 0.5f)
#define POWERUP_COLLECTION_RADIUS_SQ (POWERUP_COLLECTION_RADIUS * POWERUP_COLLECTION_RADIUS)
#define STUN_AURA_RANGE ((float)TILE_SIZE * 1.5f)
#define STUN_AURA_RANGE_SQ (STUN_AURA_RANGE * STUN_AURA_RANGE)
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
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1}, 
    {1, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}, 
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1}, 
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1}, 
    {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
};

// Listas encadeadas (alocação dinâmica)
ItemNode* itemsHead = NULL;
PowerUpNode* powerupsHead = NULL;

// Variáveis Globais PowerUp e Efeitos
int numActivePowerups = 0;
float powerupSpawnTimer = 0.0f;
PlacedTrap currentTrap = {0};
float boostTimer[2] = {0.0f}; // [0] Ladrão, [1] Policial
int characterState[2] = {0};  // [0] Ladrão, [1] Policial 

// ====== FUNÇÕES DE GERENCIAMENTO DE LISTAS ENCADEADAS ======

// Criar um novo PowerUp
PowerUpNode* PowerUp_Create(int type, Vector2 position) {
    PowerUpNode* node = (PowerUpNode*)malloc(sizeof(PowerUpNode));
    if (node) {
        node->powerup.type = type;
        node->powerup.position = position;
        node->powerup.active = 1;
        node->next = NULL;
    }
    return node;
}

// Adicionar PowerUp à lista
void PowerUp_AddToList(PowerUpNode** head, PowerUpNode* node) {
    if (!node) return;
    node->next = *head;
    *head = node;
}

// Remover PowerUp da lista
void PowerUp_RemoveFromList(PowerUpNode** head, PowerUpNode* node) {
    if (!head || !*head || !node) return;
    
    if (*head == node) {
        *head = node->next;
        free(node);
        return;
    }
    
    PowerUpNode* current = *head;
    while (current->next && current->next != node) {
        current = current->next;
    }
    
    if (current->next == node) {
        current->next = node->next;
        free(node);
    }
}

// Liberar toda a lista de PowerUps
void PowerUp_FreeList(PowerUpNode** head) {
    if (!head) return;
    PowerUpNode* current = *head;
    while (current) {
        PowerUpNode* next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

// Criar um novo Item
ItemNode* Item_Create(Vector2 position, int row, int col) {
    ItemNode* node = (ItemNode*)malloc(sizeof(ItemNode));
    if (node) {
        node->item.position = position;
        node->item.row = row;
        node->item.col = col;
        node->item.collected = 0;
        node->next = NULL;
    }
    return node;
}

// Adicionar Item à lista
void Item_AddToList(ItemNode** head, ItemNode* node) {
    if (!node) return;
    node->next = *head;
    *head = node;
}

// Liberar toda a lista de Items
void Item_FreeList(ItemNode** head) {
    if (!head) return;
    ItemNode* current = *head;
    while (current) {
        ItemNode* next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

// ====== FIM DAS FUNÇÕES DE GERENCIAMENTO ======


// Helper: movimenta um personagem com colisão
static void MoveCharacter(Character* ch, float dt){
    if (!ch->active) return;
    
    // VERIFICA SE ESTÁ IMOBILIZADO - otimizado para 2 jogadores
    int charIndex = (ch == &ladrao) ? 0 : 1;
    if (characterState[charIndex] == 1) return;
    
    // Normaliza velocidade
    if ((ch->velocity.x != 0.0f) || (ch->velocity.y != 0.0f)) {
        ch->velocity = Vector2Normalize(ch->velocity);
    }
    
    float moveSpeed = ch->speed * dt;
    
     // Colisão X
    if (ch->velocity.x != 0.0f) { 
        float futureX = ch->position.x + ch->velocity.x * moveSpeed;
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
        float futureY = ch->position.y + ch->velocity.y * moveSpeed;
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
    
    // Clamp bordas - usando cache
    ch->position.x = Clamp(ch->position.x, (float)radius, (float)SCREEN_WIDTH - (float)radius); 
    ch->position.y = Clamp(ch->position.y, (float)radius, (float)SCREEN_HEIGHT - (float)radius);
}

// Helper: Tenta spawnar um PowerUp em uma célula vazia (COM LISTA ENCADEADA)
static void TrySpawnPowerUp(int type) {
    if (numActivePowerups >= MAX_POWERUPS) return;

    // Alocação estática otimizada
    static int freeCells[100][2];
    int freeCount = 0;
    
    // Encontrar células livres
    for (int row = 1; row < MAP_ROWS - 1 && freeCount < 100; row++) {
        for (int col = 1; col < MAP_COLS - 1 && freeCount < 100; col++) {
            if (gameMap[row][col] != TILE_WALL && gameMap[row][col] != TILE_ITEM) {
                
                // Verifica se já existe um powerup no local usando lista encadeada
                int occupied = 0;
                PowerUpNode* current = powerupsHead;
                while (current != NULL) {
                    if (current->powerup.active) {
                        int p_row = (int)(current->powerup.position.y / TILE_SIZE);
                        int p_col = (int)(current->powerup.position.x / TILE_SIZE);
                        if (p_row == row && p_col == col) {
                            occupied = 1;
                            break;
                        }
                    }
                    current = current->next;
                }
                if (occupied) continue;

                freeCells[freeCount][0] = row;
                freeCells[freeCount][1] = col;
                freeCount++;
            }
        }
    }

    if (freeCount > 0) {
        int randomIndex = rand() % freeCount;
        int row = freeCells[randomIndex][0];
        int col = freeCells[randomIndex][1];
        
        // Criar PowerUp com alocação dinâmica
        Vector2 powerupPos = {
            (float)col * TILE_SIZE + TILE_SIZE / 2.0f,
            (float)row * TILE_SIZE + TILE_SIZE / 2.0f
        };
        PowerUpNode* newPowerup = PowerUp_Create(type, powerupPos);
        
        if (newPowerup != NULL) {
            PowerUp_AddToList(&powerupsHead, newPowerup);
            numActivePowerups++;
        }
    } 
}

void InitGame(void){
    
    float current_radius = (float)TILE_SIZE / 2.0f;

    // Inicializa ladrão: Posição no canto inferior direito (longe do policial)
    // Linha 15, Coluna 27 é uma posição vazia (0) no mapa
    ladrao.position = (Vector2){(float)27 * TILE_SIZE + current_radius, (float)15 * TILE_SIZE + current_radius};
    ladrao.speed = 180.0f;
    ladrao.active = 1;
    
    // Inicializa policial 
    policial.position = (Vector2){(float)2 * TILE_SIZE + current_radius, (float)1 * TILE_SIZE + current_radius};
    policial.speed = 170.0f;
    policial.active = 1;
    policial.playerIndex = 0;
    
    // === INICIALIZA ITENS COM LISTA ENCADEADA (alocação dinâmica) ===
    Item_FreeList(&itemsHead); // Libera lista anterior se existir
    numItems = 0;
    itemsCollected = 0;
    
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            if (gameMap[row][col] == TILE_ITEM) {
                Vector2 itemPos = {
                    (float)col * TILE_SIZE + TILE_SIZE / 2.0f,
                    (float)row * TILE_SIZE + TILE_SIZE / 2.0f
                };
                ItemNode* newItem = Item_Create(itemPos, row, col);
                
                if (newItem != NULL) {
                    Item_AddToList(&itemsHead, newItem);
                    numItems++;
                }
            }
        }
    }

    // === Inicializa POWERUPS/EFEITOS ===
    PowerUp_FreeList(&powerupsHead); // Libera powerups anteriores
    powerupSpawnTimer = (float)GetRandomValue(4, 8);
    numActivePowerups = 0;
    currentTrap.active = 0;
    boostTimer[0] = 0.0f;
    boostTimer[1] = 0.0f;
    characterState[0] = 0; // Estado normal
    characterState[1] = 0;
    
    gameTimer = INITIAL_TIME;
    gameResult = 0;
}

void UpdateGame(float dt){
    if (currentScreen == MENU) {
        if (IsKeyPressed(KEY_ENTER)) {
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
        // Prioriza movimento vertical sobre horizontal para evitar diagonal
        if(IsKeyDown(KEY_W)) {
            ladrao.velocity.y -= 1.0f;
        } else if(IsKeyDown(KEY_S)) {
            ladrao.velocity.y += 1.0f;
        } else if(IsKeyDown(KEY_A)) {
            ladrao.velocity.x -= 1.0f;
        } else if(IsKeyDown(KEY_D)) {
            ladrao.velocity.x += 1.0f;
        }
        
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
                 // Verifica se atinge o policial dentro do raio da aura
                 float distance = Vector2Distance(ladrao.position, policial.position);
                 if (policial.active && distance < (float)TILE_SIZE * 1.5f) { // Alcance da aura
                    if (characterState[1] == 0) { // Se o policial não estiver já stunado
                        characterState[1] = 1; // Policial atordoado
                        boostTimer[1] = POWERUP_DURATION_STUN; // Aplica o stun (3s)
                        
                        // Gasta o poder na primeira ativação de stun
                        characterState[0] = 0; 
                        boostTimer[0] = 0.0f;
                    }
                 }
             }
        }

        MoveCharacter(&ladrao, dt);
        
        // Policial
        policial.speed = 170.0f;
        if (boostTimer[1] > 0.0f) {
            // Policial pode ter BOOST ou estar STUNNED. Se estiver STUNNED (state 1), a velocidade é 0.
            if (characterState[1] == 0) {
                policial.speed = 270.0f; // Boost ativo
            }
            boostTimer[1] -= dt;
        } 
        
        // Tempo de STUN do Policial (se for 1)
        if (characterState[1] == 1) { 
            policial.speed = 0.0f; // Imobilizado
            // O timer é controlado pelo boostTimer[1]
            if (boostTimer[1] <= 0.0f) characterState[1] = 0; // Fim do stun
        }
        
        policial.velocity = (Vector2){0.0f, 0.0f};
        // Controles do Policial (setas) - Prioriza movimento vertical para evitar diagonal
        if (IsKeyDown(KEY_UP)) {
            policial.velocity.y -= 1.0f;
        } else if (IsKeyDown(KEY_DOWN)) {
            policial.velocity.y += 1.0f;
        } else if (IsKeyDown(KEY_LEFT)) {
            policial.velocity.x -= 1.0f;
        } else if (IsKeyDown(KEY_RIGHT)) {
            policial.velocity.x += 1.0f;
        }

        MoveCharacter(&policial, dt);
        
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

        // === COLETA DE POWERUPS (LISTA ENCADEADA) ===
        PowerUpNode* currentPowerup = powerupsHead;
        
        while (currentPowerup != NULL) {
            if (!currentPowerup->powerup.active) {
                currentPowerup = currentPowerup->next;
                continue;
            }
            
            int collected = 0;
            int character_index = -1;
            
            // Ladrão coleta
            float dx = ladrao.position.x - currentPowerup->powerup.position.x;
            float dy = ladrao.position.y - currentPowerup->powerup.position.y;
            if ((dx * dx + dy * dy) < POWERUP_COLLECTION_RADIUS_SQ) {
                character_index = 0;
                collected = 1;
            } else {
                // Policial coleta
                dx = policial.position.x - currentPowerup->powerup.position.x;
                dy = policial.position.y - currentPowerup->powerup.position.y;
                if ((dx * dx + dy * dy) < POWERUP_COLLECTION_RADIUS_SQ) {
                    character_index = 1;
                    collected = 1;
                }
            }

            if (collected) {
                if (currentPowerup->powerup.type == POWERUP_BOOST) {
                    boostTimer[character_index] = POWERUP_DURATION_BOOST; 
                } else if (currentPowerup->powerup.type == POWERUP_STUN_BOMB && character_index == 0) { 
                    characterState[0] = 2;
                    boostTimer[0] = POWERUP_DURATION_BOOST;
                } else if (currentPowerup->powerup.type == POWERUP_TRAP && character_index == 1) { 
                    currentTrap.position = currentPowerup->powerup.position;
                    currentTrap.active = 1;
                    characterState[1] = 0;
                } else if (currentPowerup->powerup.type == POWERUP_STUN_BOMB && character_index > 0) {
                    collected = 0;
                }

                if (collected) {
                    PowerUpNode* toRemove = currentPowerup;
                    currentPowerup = currentPowerup->next;
                    PowerUp_RemoveFromList(&powerupsHead, toRemove);
                    numActivePowerups--;
                    continue;
                }
            }
            
            currentPowerup = currentPowerup->next;
        }
        
        // === COLETA DE ITENS (LISTA ENCADEADA) ===
        ItemNode* currentItem = itemsHead;
        while (currentItem != NULL) {
            if (!currentItem->item.collected) {
                float dx = ladrao.position.x - currentItem->item.position.x;
                float dy = ladrao.position.y - currentItem->item.position.y;
                if ((dx * dx + dy * dy) < ITEM_COLLECTION_RADIUS_SQ) { 
                    currentItem->item.collected = 1;
                    itemsCollected++;
                    
                    if (itemsCollected >= numItems) {
                        currentScreen = END_GAME;
                        gameResult = 0; 
                        lastScore = numItems * 100 + (int)(gameTimer * 5.0f); 
                        playerName[0] = '\0';
                        enteringName = 1;
                        return;
                    }
                }
            }
            currentItem = currentItem->next;
        }
        
        // === DETECÇÃO DE CAPTURA ===
        float distance = Vector2Distance(ladrao.position, policial.position);
        // Captura só acontece se nenhum dos dois estiver atordoado
        if (distance < COLLISION_DISTANCE && characterState[0] == 0 && characterState[1] == 0) {
            currentScreen = END_GAME;
            gameResult = 1;
            lastScore = (int)((INITIAL_TIME - gameTimer) * 10.0f);
            playerName[0] = '\0'; 
            enteringName = 1;
            return;
        }
        
        // === CRONÔMETRO (mantido) ===
        gameTimer -= dt;
        if (gameTimer <= 0.0f) {
            gameTimer = 0.0f;
            currentScreen = END_GAME;
            gameResult = 0;
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
            DrawText("Pressione ENTER para iniciar", GetScreenWidth()/2 - 200, 300, 25, RAYWHITE);
            
            DrawText("CONTROLES:", GetScreenWidth()/2 - 100, 480, 20, YELLOW);
            
            DrawText("Ladrao: WASD | Especial: STUN AURA (Ativado ao coletar o item - 5s de efeito)", 100, 530, 18, RAYWHITE);
            
            DrawText("Policial: SETAS | Especial: TRAP (Ativada no chao ao coletar o item)", 100, 560, 18, RAYWHITE);
            
        } else if (currentScreen == GAMEPLAY) {
            // === DESENHAR O MAPA ===
            int useWallTexture = (texturawall.width > 0 && texturawall.height > 0);
            Rectangle srcWall = {0, 0, (float)texturawall.width, (float)texturawall.height};
            
            for (int row = 0; row < MAP_ROWS; row++) {
                for (int col = 0; col < MAP_COLS; col++) {
                    Rectangle destRec = {(float)col * TILE_SIZE, (float)row * TILE_SIZE, (float)TILE_SIZE, (float)TILE_SIZE};
                    if (gameMap[row][col] == TILE_WALL) {
                        if (useWallTexture) {
                            DrawTexturePro(texturawall, srcWall, destRec, (Vector2){0, 0}, 0.0f, WHITE);
                        } else {
                            DrawRectangleRec(destRec, ULTRA_DARKBLUE);
                        }
                    } else {
                        DrawRectangleRec(destRec, chao); 
                    }
                }
            }

            // === DESENHAR ITENS COLETÁVEIS (LISTA ENCADEADA) ===
            float itemSize = (float)TILE_SIZE * 0.9375f;  // Aumentado de 0.75f para 0.9375f (25% maior)
            int useKeyTexture = (texturakey.width > 0 && texturakey.height > 0);
            
            ItemNode* currentItem = itemsHead;
            while (currentItem != NULL) {
                if (!currentItem->item.collected) {
                    if (useKeyTexture) {
                        // Calcula dimensões mantendo a proporção da textura
                        float aspectRatio = (float)texturakey.width / (float)texturakey.height;
                        float drawWidth, drawHeight;
                        
                        if (aspectRatio > 1.0f) {
                            drawWidth = itemSize;
                            drawHeight = itemSize / aspectRatio;
                        } else {
                            drawHeight = itemSize;
                            drawWidth = itemSize * aspectRatio;
                        }
                        
                        DrawTexturePro(
                            texturakey,
                            (Rectangle){ 0, 0, (float)texturakey.width, (float)texturakey.height },
                            (Rectangle){ 
                                currentItem->item.position.x - drawWidth * 0.5f, 
                                currentItem->item.position.y - drawHeight * 0.5f, 
                                drawWidth, 
                                drawHeight 
                            },
                            (Vector2){ 0, 0 },
                            0.0f,
                            WHITE
                        );
                    } else {
                        int itemX = (int)currentItem->item.position.x;
                        int itemY = (int)currentItem->item.position.y;
                        DrawCircle(itemX, itemY, (float)TILE_SIZE * 0.21f, GOLD);
                        DrawCircle(itemX, itemY, (float)TILE_SIZE * 0.14f, YELLOW);
                    }
                }
                currentItem = currentItem->next;
            }
            
            // === DESENHAR POWERUPS (LISTA ENCADEADA) ===
            float puSize = (float)TILE_SIZE * 1.5f;  // Aumentado de 0.6f para 1.5f
            
            PowerUpNode* currentPowerup = powerupsHead;
            while (currentPowerup != NULL) {
                if (currentPowerup->powerup.active) {
                    Texture2D* tex = NULL;
                    Color color = RAYWHITE;
                    char symbol = '?';
                    
                    if (currentPowerup->powerup.type == POWERUP_BOOST) { 
                        tex = &texturapower_boost; 
                        color = LIME; 
                        symbol = 'S'; 
                    } else if (currentPowerup->powerup.type == POWERUP_STUN_BOMB) { 
                        tex = &texturapower_stun; 
                        color = BLUE; 
                        symbol = 'A'; 
                    } else if (currentPowerup->powerup.type == POWERUP_TRAP) { 
                        tex = &texturapower_trap; 
                        color = RED; 
                        symbol = 'T'; 
                    }
                    
                    // Debug: verifica se a textura é válida
                    static int debugCount = 0;
                    if (debugCount < 5 && tex != NULL) {
                        printf("PowerUp tipo=%d: tex=%p, width=%d, height=%d\n", 
                               currentPowerup->powerup.type, (void*)tex, tex->width, tex->height);
                        debugCount++;
                    }

                    if (tex != NULL && tex->width > 0 && tex->height > 0) {
                        // Calcula dimensões mantendo a proporção da textura
                        float aspectRatio = (float)tex->width / (float)tex->height;
                        float drawWidth, drawHeight;
                        
                        if (aspectRatio > 1.0f) {
                            // Textura mais larga que alta
                            drawWidth = puSize;
                            drawHeight = puSize / aspectRatio;
                        } else {
                            // Textura mais alta que larga (ou quadrada)
                            drawHeight = puSize;
                            drawWidth = puSize * aspectRatio;
                        }
                        
                        DrawTexturePro(
                            *tex,
                            (Rectangle){ 0, 0, (float)tex->width, (float)tex->height },
                            (Rectangle){ 
                                currentPowerup->powerup.position.x - drawWidth * 0.5f, 
                                currentPowerup->powerup.position.y - drawHeight * 0.5f, 
                                drawWidth, 
                                drawHeight 
                            },
                            (Vector2){ 0, 0 },
                            0.0f,
                            WHITE
                        );
                    } else {
                        char buf[2] = {symbol, '\0'};
                        DrawText(buf, (int)currentPowerup->powerup.position.x - 5, (int)currentPowerup->powerup.position.y - 10, 20, color);
                    }
                }
                currentPowerup = currentPowerup->next;
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
                float ringSize = spriteCenter + 5.0f + ((int)(GetTime() * 10) % 10);
                DrawRing(ladrao.position, spriteCenter + 5.0f, ringSize, 0, 360, 16, Fade(BLUE, 0.5f));
                
                char auraText[32];
                sprintf(auraText, "STUN AURA: %.1fs", boostTimer[0]);
                DrawText(auraText, (int)ladrao.position.x - 60, (int)ladrao.position.y - 60, 15, BLUE);
            }

            DrawTexturePro(
                texturaladrao, 
                (Rectangle){ 0, 0, texturaladrao.width, texturaladrao.height },
                (Rectangle){ ladrao.position.x, ladrao.position.y, spriteSize, spriteSize },
                (Vector2){ spriteCenter, spriteCenter },
                0.0f, 
                ladraoTint 
            );
            
            // POLICIAL
            Color pTint = BLUE;
            if (boostTimer[1] > 0.0f) pTint = GREEN;
            if (characterState[1] == 1) pTint = PURPLE; // Imobilizado

            DrawTexturePro(
                texturapolicia, 
                (Rectangle){ 0, 0, texturapolicia.width, texturapolicia.height }, 
                (Rectangle){ policial.position.x, policial.position.y, spriteSize, spriteSize },
                (Vector2){ spriteCenter, spriteCenter }, 
                0.0f, 
                pTint
            );

            // === HUD ===
            char timerText[32];
            sprintf(timerText, "TEMPO: %.1fs", gameTimer);
            DrawText(timerText, 10, 10, 20, WHITE);
            
            char itemsText[64];
            sprintf(itemsText, "Itens: %d/%d", itemsCollected, numItems);
            DrawText(itemsText, 10, 35, 18, GOLD); 
            
            char powerupText[64];
            sprintf(powerupText, "Proximo PowerUp em: %.1fs", powerupSpawnTimer);
            DrawText(powerupText, GetScreenWidth() - 300, 10, 18, RAYWHITE);
            
        } else if (currentScreen == END_GAME) {
            const char* message;
            Color messageColor;
            char finalMessage[128] = "";
            
            if (gameResult == 1) {
                sprintf(finalMessage, "CAPTURADO! Policial VENCEU! Score: %d", lastScore);
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
#include "../include/jogo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

// Implementação de ranking usando lista encadeada dinâmica
// Cada nó guarda um ScoreEntry e o ponteiro next

typedef struct RankNode {
    ScoreEntry entry;
    struct RankNode* next;
} RankNode;

// Cabeças e contadores para Policiais e Ladrão
static RankNode* rankingPolicia_head = NULL;
static int rankingPolicia_count = 0;

static RankNode* rankingLadrao_head = NULL;
static int rankingLadrao_count = 0;

// Helpers
static RankNode* create_node(const char* name, int score){
    RankNode* n = (RankNode*)malloc(sizeof(RankNode));
    if (!n) return NULL;
    strncpy(n->entry.name, name ? name : "ANON", RANKING_NAME_MAX);
    n->entry.name[RANKING_NAME_MAX] = '\0';
    n->entry.score = score;
    n->next = NULL;
    return n;
}

static void free_list(RankNode** head){
    RankNode* cur = *head;
    while (cur) {
        RankNode* nx = cur->next;
        free(cur);
        cur = nx;
    }
    *head = NULL;
}

// Insere o nó em ordem decrescente (score maior primeiro)
static void insert_sorted(RankNode** head, RankNode* node){
    if (!head || !node) return;
    if (*head == NULL || node->entry.score > (*head)->entry.score){
        node->next = *head;
        *head = node;
        return;
    }
    RankNode* cur = *head;
    while (cur->next && cur->next->entry.score >= node->entry.score) {
        cur = cur->next;
    }
    node->next = cur->next;
    cur->next = node;
}

// Remove último nó (menor score). Retorna 1 se removeu, 0 se lista vazia
static int remove_last(RankNode** head){
    if (!head || !*head) return 0;
    RankNode* cur = *head;
    RankNode* prev = NULL;
    while (cur->next){ prev = cur; cur = cur->next; }
    if (!prev){ // só um
        free(cur);
        *head = NULL;
    } else {
        prev->next = NULL;
        free(cur);
    }
    return 1;
}

// ===== POLICIA =====
void Ranking_LoadPolicia(void){
    free_list(&rankingPolicia_head);
    rankingPolicia_count = 0;
    FILE *f = fopen("ranking_policia.txt", "r");
    if (!f) return;

    char name[RANKING_NAME_MAX+1];
    int score;
    while (rankingPolicia_count < RANKING_SIZE && fscanf(f, "%31s %d\n", name, &score) == 2){
        RankNode* n = create_node(name, score);
        if (!n) break;
        insert_sorted(&rankingPolicia_head, n);
        rankingPolicia_count++;
    }
    fclose(f);
}

void Ranking_SavePolicia(void){
    FILE *f = fopen("ranking_policia.txt", "w");
    if (!f) return;
    RankNode* cur = rankingPolicia_head;
    while (cur){
        fprintf(f, "%s %d\n", cur->entry.name, cur->entry.score);
        cur = cur->next;
    }
    fclose(f);
}

void Ranking_AddPolicia(const char* name, int score){
    const char* used = (name && name[0]) ? name : "ANON";
    RankNode* n = create_node(used, score);
    if (!n) return;
    insert_sorted(&rankingPolicia_head, n);
    rankingPolicia_count++;
    if (rankingPolicia_count > RANKING_SIZE){
        // remove último
        remove_last(&rankingPolicia_head);
        rankingPolicia_count = RANKING_SIZE;
    }
    Ranking_SavePolicia();
}

void Ranking_DrawPolicia(int x, int y){
    DrawText("RANKING - POLICIAL", x, y, 20, GOLD);
    if (!rankingPolicia_head){
        DrawText("(vazio)", x, y + 30, 18, RAYWHITE);
        return;
    }
    RankNode* cur = rankingPolicia_head;
    int i = 0;
    char buf[256];
    while (cur && i < RANKING_SIZE){
        sprintf(buf, "%2d. %-10s  %5d", i+1, cur->entry.name, cur->entry.score);
        DrawText(buf, x, y + 30 + i*22, 18, RAYWHITE);
        cur = cur->next; i++;
    }
}

// ===== LADRÃO =====
void Ranking_LoadLadrao(void){
    free_list(&rankingLadrao_head);
    rankingLadrao_count = 0;
    FILE *f = fopen("ranking_ladrao.txt", "r");
    if (!f) return;

    char name[RANKING_NAME_MAX+1];
    int score;
    while (rankingLadrao_count < RANKING_SIZE && fscanf(f, "%31s %d\n", name, &score) == 2){
        RankNode* n = create_node(name, score);
        if (!n) break;
        insert_sorted(&rankingLadrao_head, n);
        rankingLadrao_count++;
    }
    fclose(f);
}

void Ranking_SaveLadrao(void){
    FILE *f = fopen("ranking_ladrao.txt", "w");
    if (!f) return;
    RankNode* cur = rankingLadrao_head;
    while (cur){
        fprintf(f, "%s %d\n", cur->entry.name, cur->entry.score);
        cur = cur->next;
    }
    fclose(f);
}

void Ranking_AddLadrao(const char* name, int score){
    const char* used = (name && name[0]) ? name : "ANON";
    RankNode* n = create_node(used, score);
    if (!n) return;
    insert_sorted(&rankingLadrao_head, n);
    rankingLadrao_count++;
    if (rankingLadrao_count > RANKING_SIZE){
        remove_last(&rankingLadrao_head);
        rankingLadrao_count = RANKING_SIZE;
    }
    Ranking_SaveLadrao();
}

void Ranking_DrawLadrao(int x, int y){
    DrawText("RANKING - LADRAO", x, y, 20, GREEN);
    if (!rankingLadrao_head){
        DrawText("(vazio)", x, y + 30, 18, RAYWHITE);
        return;
    }
    RankNode* cur = rankingLadrao_head;
    int i = 0;
    char buf[256];
    while (cur && i < RANKING_SIZE){
        sprintf(buf, "%2d. %-10s  %5d", i+1, cur->entry.name, cur->entry.score);
        DrawText(buf, x, y + 30 + i*22, 18, RAYWHITE);
        cur = cur->next; i++;
    }
}
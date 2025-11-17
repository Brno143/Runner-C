#include "../include/jogo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "raylib.h"

// Arrays separados para cada ranking
static ScoreEntry rankingPolicia[RANKING_SIZE];
static int rankingPolicia_count = 0;

static ScoreEntry rankingLadrao[RANKING_SIZE];
static int rankingLadrao_count = 0;

// Helper: sort descending
static int cmp_desc(const void* a, const void* b){
    const ScoreEntry* sa = (const ScoreEntry*)a;
    const ScoreEntry* sb = (const ScoreEntry*)b;
    return sb->score - sa->score;
}

// ===== FUNÇÕES DO RANKING DO POLICIAL =====
void Ranking_LoadPolicia(void){
    FILE *f = fopen("ranking_policia.txt", "r");
    rankingPolicia_count = 0;
    if (!f) return; // se não existe, inicia vazio

    while (rankingPolicia_count < RANKING_SIZE && !feof(f)){
        char name[RANKING_NAME_MAX+1];
        int score;
        if (fscanf(f, "%31s %d\n", name, &score) == 2){
            strncpy(rankingPolicia[rankingPolicia_count].name, name, RANKING_NAME_MAX);
            rankingPolicia[rankingPolicia_count].name[RANKING_NAME_MAX] = '\0';
            rankingPolicia[rankingPolicia_count].score = score;
            rankingPolicia_count++;
        } else break;
    }
    fclose(f);
    if (rankingPolicia_count>0) qsort(rankingPolicia, rankingPolicia_count, sizeof(ScoreEntry), cmp_desc);
}

void Ranking_SavePolicia(void){
    FILE *f = fopen("ranking_policia.txt", "w");
    if (!f) return;
    for (int i=0;i<rankingPolicia_count;i++){
        fprintf(f, "%s %d\n", rankingPolicia[i].name, rankingPolicia[i].score);
    }
    fclose(f);
}

void Ranking_AddPolicia(const char* name, int score){
    // Se nome vazio, usa ANON
    const char* used = (name && name[0]) ? name : "ANON";

    // Se há menos do que RANKING_SIZE, adiciona direto
    if (rankingPolicia_count < RANKING_SIZE){
        strncpy(rankingPolicia[rankingPolicia_count].name, used, RANKING_NAME_MAX);
        rankingPolicia[rankingPolicia_count].name[RANKING_NAME_MAX] = '\0';
        rankingPolicia[rankingPolicia_count].score = score;
        rankingPolicia_count++;
    } else {
        // Se o novo score for melhor que o menor, substitui
        int minIdx = 0;
        for (int i=1;i<rankingPolicia_count;i++){
            if (rankingPolicia[i].score < rankingPolicia[minIdx].score) minIdx = i;
        }
        if (score > rankingPolicia[minIdx].score){
            strncpy(rankingPolicia[minIdx].name, used, RANKING_NAME_MAX);
            rankingPolicia[minIdx].name[RANKING_NAME_MAX] = '\0';
            rankingPolicia[minIdx].score = score;
        } else {
            return;
        }
    }
    qsort(rankingPolicia, rankingPolicia_count, sizeof(ScoreEntry), cmp_desc);
    Ranking_SavePolicia();
}

void Ranking_DrawPolicia(int x, int y){
    DrawText("RANKING - POLICIAL", x, y, 20, GOLD);
    for (int i=0;i<rankingPolicia_count;i++){
        char buf[256];
        sprintf(buf, "%2d. %-10s  %5d", i+1, rankingPolicia[i].name, rankingPolicia[i].score);
        DrawText(buf, x, y + 30 + i*22, 18, RAYWHITE);
    }
    if (rankingPolicia_count == 0){
        DrawText("(vazio)", x, y + 30, 18, RAYWHITE);
    }
}

// ===== FUNÇÕES DO RANKING DO LADRÃO =====
void Ranking_LoadLadrao(void){
    FILE *f = fopen("ranking_ladrao.txt", "r");
    rankingLadrao_count = 0;
    if (!f) return;

    while (rankingLadrao_count < RANKING_SIZE && !feof(f)){
        char name[RANKING_NAME_MAX+1];
        int score;
        if (fscanf(f, "%31s %d\n", name, &score) == 2){
            strncpy(rankingLadrao[rankingLadrao_count].name, name, RANKING_NAME_MAX);
            rankingLadrao[rankingLadrao_count].name[RANKING_NAME_MAX] = '\0';
            rankingLadrao[rankingLadrao_count].score = score;
            rankingLadrao_count++;
        } else break;
    }
    fclose(f);
    if (rankingLadrao_count>0) qsort(rankingLadrao, rankingLadrao_count, sizeof(ScoreEntry), cmp_desc);
}

void Ranking_SaveLadrao(void){
    FILE *f = fopen("ranking_ladrao.txt", "w");
    if (!f) return;
    for (int i=0;i<rankingLadrao_count;i++){
        fprintf(f, "%s %d\n", rankingLadrao[i].name, rankingLadrao[i].score);
    }
    fclose(f);
}

void Ranking_AddLadrao(const char* name, int score){
    const char* used = (name && name[0]) ? name : "ANON";

    if (rankingLadrao_count < RANKING_SIZE){
        strncpy(rankingLadrao[rankingLadrao_count].name, used, RANKING_NAME_MAX);
        rankingLadrao[rankingLadrao_count].name[RANKING_NAME_MAX] = '\0';
        rankingLadrao[rankingLadrao_count].score = score;
        rankingLadrao_count++;
    } else {
        int minIdx = 0;
        for (int i=1;i<rankingLadrao_count;i++){
            if (rankingLadrao[i].score < rankingLadrao[minIdx].score) minIdx = i;
        }
        if (score > rankingLadrao[minIdx].score){
            strncpy(rankingLadrao[minIdx].name, used, RANKING_NAME_MAX);
            rankingLadrao[minIdx].name[RANKING_NAME_MAX] = '\0';
            rankingLadrao[minIdx].score = score;
        } else {
            return;
        }
    }
    qsort(rankingLadrao, rankingLadrao_count, sizeof(ScoreEntry), cmp_desc);
    Ranking_SaveLadrao();
}

void Ranking_DrawLadrao(int x, int y){
    DrawText("RANKING - LADRAO", x, y, 20, GREEN);
    for (int i=0;i<rankingLadrao_count;i++){
        char buf[256];
        sprintf(buf, "%2d. %-10s  %5d", i+1, rankingLadrao[i].name, rankingLadrao[i].score);
        DrawText(buf, x, y + 30 + i*22, 18, RAYWHITE);
    }
    if (rankingLadrao_count == 0){
        DrawText("(vazio)", x, y + 30, 18, RAYWHITE);
    }
}
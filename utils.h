#ifndef _UTILS_HH_
#define _UTILS_HH_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// --- Structs ----------------------------------------------------------------

struct keysPredict {
    struct node* first;
    int totalKeys;
    int totalWords;
};

struct node {
    char character;
    struct node* next;
    int end;
    char* word;
    struct node* down;
};

//Estructura creada para resolver keysPredictRun de forma iterativa simil BFS
struct impostor_node {
    char character;
    struct node* next;
    int end;
    char* word;
    struct node* down;
    struct impostor_node* fake_next;
};

// --- Keys Predict --------------------------------------------------------------

struct keysPredict* keysPredictNew();

void keysPredictAddWord(struct keysPredict* kt, char* word);

void keysPredictRemoveWord(struct keysPredict* kt, char* word);

struct node* keysPredictFind(struct keysPredict* kt, char* word);

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount);

char** keysPredictRun_v2(struct keysPredict* kt, char* partialWord, int* wordsCount);

char** keysPredictListAll(struct keysPredict* kt, int* wordsCount);

void keysPredictDelete(struct keysPredict* kt);

void keysPredictPrint(struct keysPredict* kt);

// --- Auxiliar functions -----------------------------------------------------

void keysPredictPrintAux(struct node* n, int level);

struct node* findNodeInLevel(struct node** list, char character);

struct node* addSortedNewNodeInLevel(struct node** list, char character);

void deleteArrayOfWords(char** words, int wordsCount);

// FUNCIONES QUE NO ESTABAN EN EL ENUNCIADO
void printList(struct node** list); // para debug

void printWords(char** words, int wordsCount); // para debug

void encontrarPalabras(struct node* curr, int* wordsCount, struct node** found, struct node** found_index); // para keysPredictRun

char** makeArrayFromList(struct node* found, int wordsCount); // para keysPredictRun

char** makeArrayFromImpostorList(struct impostor_node* found, int wordsCount); // para keysPredictRun_v2

struct impostor_node* impostorDup(struct impostor_node* n); // para keysPredictRun_v2

struct impostor_node* impostorFromNode(struct node* n); // para keysPredictRun_v2

void borrarRecursiva(struct node* n); // para keysPredictDelete
// --- Strings ----------------------------------------------------------------

int strLen(char* src);

char* strDup(char* src);

// esta no se pedia en el enunciado
int strCmp(char* src1, char* src2);

#endif

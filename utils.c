#include "utils.h"

int strLen(char* src) {
    int len = 0;
    while(src[len] != '\0') {
        len++;
    }
    return len;
}

char* strDup(char* src) {
    char* rv = (char*)malloc(sizeof(char)*(strLen(src)+1));
    for(int i = 0; i < strLen(src)+1; i++) {
        rv[i] = src[i];
    }
    return rv;
}

// Keys Predict

struct keysPredict* keysPredictNew() {
    struct keysPredict* kt = (struct keysPredict*)malloc(sizeof(struct keysPredict));
    kt->first = 0;
    kt->totalKeys = 0;
    kt->totalWords = 0;
    return kt;
}

void keysPredictAddWord(struct keysPredict* kt, char* word) {

    // COMPLETAR

}

void keysPredictRemoveWord(struct keysPredict* kt, char* word) {

    // COMPLETAR
}

struct node* keysPredictFind(struct keysPredict* kt, char* word) {

    // COMPLETAR

    return 0;
}

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {

    // COMPLETAR

    return 0;
}

int keysPredictCountWordAux(struct node* n) {

    // COMPLETAR
}

char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {

    // COMPLETAR

    return 0;
}

void keysPredictDelete(struct keysPredict* kt) {

    // COMPLETAR

}

void keysPredictPrint(struct keysPredict* kt) {
    printf("--- Predict --- Keys: %i Words: %i\n", kt->totalKeys, kt->totalWords);
    keysPredictPrintAux(kt->first, 0);
    printf("---\n");
}

void keysPredictPrintAux(struct node* n, int level) {
    if(!n) return;
    struct node* current = n;
    while(current) {
        for(int i=0; i<level;i++) printf(" |   ");
        if(current->end) {
            printf("[%c]\n",current->character);
        } else  {
            printf(" %c \n",current->character);
        }
        keysPredictPrintAux(current->down, level+1);
        current = current->next;
    }
}

// Auxiliar functions

void printList(struct node** list) {
    struct node* current = *list;
    while (current != NULL) {
        printf("%c, ", current->character);
        current = current->next;
    }
    printf("\n");
}

struct node* findNodeInLevel(struct node** list, char character) {
    int i = 0;
    struct node* curr = *list;
    while (curr != NULL && curr->character != character) {
        curr = curr->next;
    }

    return curr;
}

struct node* addSortedNewNodeInLevel(struct node** list, char character) {
    int i = 0;
    struct node* curr = *list;
    struct node* prev = 0;
    struct node* nuevo = (struct node*)malloc(sizeof(struct node));
    nuevo->character = character;
    nuevo->end = 0;
    nuevo->word = 0;
    nuevo->down = 0;
    while (curr != 0 && curr->character < character) {
        prev = curr;
        curr = curr->next;
    }
    if (prev == 0) {
        nuevo->next = *list;
        *list = nuevo;
    } else {
        nuevo->next = curr;
        prev->next = nuevo;
    }
    return nuevo;
}

void deleteArrayOfWords(char** words, int wordsCount) {
    char* current;
    int i = 0;
    while (i < wordsCount) {
        current = words[i];
        free(current);
        i++;
    }
    free(words);
}

int main() {
    // Pruebas para strLen (Ejercicio 1)
    printf("Test strLen:\n");
    char* testStrings[] = {"", "a", "abc123", "long test string"};
    for (int i = 0; i < 4; i++) {
        printf("String: '%s' - Length: %d\n", testStrings[i], strLen(testStrings[i]));
    }

    // Pruebas para strDup (Ejercicio 1)
    printf("\nTest strDup:\n");
    for (int i = 0; i < 4; i++) {
        char* duplicated = strDup(testStrings[i]);
        printf("Original: '%s' - Duplicated: '%s'\n", testStrings[i], duplicated);
        free(duplicated);
    }

    // Pruebas para findNodeInLevel (Ejercicio 2)
    printf("\nTest findNodeInLevel:\n");
    struct node* nodo1 = (struct node*) malloc(sizeof(struct node));
    struct node* nodo2 = (struct node*) malloc(sizeof(struct node));
    struct node* nodo3 = (struct node*) malloc(sizeof(struct node));
    nodo1->character = 'b'; nodo1->next = nodo2;
    nodo2->character = 'e'; nodo2->next = nodo3;
    nodo3->character = 'g'; nodo3->next = NULL;
    struct node** nodeList = &nodo1;

    struct node* found = findNodeInLevel(nodeList, 'a');
    if (found != NULL) {
        printf("Found node with character: %c\n", found->character);
    } else {
        printf("Character %c not found.\n", 'a');
    }
    found = findNodeInLevel(nodeList, 'b');
    if (found != NULL) {
        printf("Found node with character: %c\n", found->character);
    } else {
        printf("Character not found.\n");
    }
    found = findNodeInLevel(nodeList, 'e');
    if (found != NULL) {
        printf("Found node with character: %c\n", found->character);
    } else {
        printf("Character not found.\n");
    }
    found = findNodeInLevel(nodeList, 'g');
    if (found != NULL) {
        printf("Found node with character: %c\n", found->character);
    } else {
        printf("Character not found.\n");
    }

    // Pruebas para addSortedNewNodeInLevel (Ejercicio 2)
    printf("\nTest addSortedNewNodeInLevel:\n");
    struct node* nuevo = addSortedNewNodeInLevel(nodeList, 'a');
    nuevo = addSortedNewNodeInLevel(nodeList, 'c');
    nuevo = addSortedNewNodeInLevel(nodeList, 'm');
    printList(nodeList);

    // Pruebas para deleteArrayOfWords (Ejercicio 2)
    printf("\nTest deleteArrayOfWords:\n");
    char** words = (char**) malloc(3 * sizeof(char*));
    words[0] = strDup("hello");
    words[1] = strDup("world");
    words[2] = strDup("test");
    for (int i = 0; i < 3; i++) {
        printf("Word %d: %s\n", i, words[i]);
    }
    deleteArrayOfWords(words, 3);  // Borra el arreglo y las strings

    return 0;
}
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

int strCmp(char* src1, char* src2) {
    if(strLen(src1) != strLen(src2)) {
        return 0;
    }
    int i = 0;
    while (i < strLen(src1) && i < strLen(src2)) {
        if (src1[i] != src2[i]) {
            return 0;
        }
        i++;
    }
    return 1;
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
    if (strLen(word) == 0) {
        return;
    }
    struct node* current = kt->first;
    struct node* prev = NULL;
    int i = 0;
    while (word[i] != '\0') {
        if (current == NULL) {
            current = addSortedNewNodeInLevel(&current, word[i]);
            if (prev != NULL){
                prev->down = current;
            }else{
                kt->first = current;
            }
            kt->totalKeys++;
        } else {
            struct node* found = findNodeInLevel(&current, word[i]);
            if (found == NULL) {
                struct node* first = addSortedNewNodeInLevel(&current, word[i]);
                if (prev != NULL){
                    prev->down = current;
                }else{
                    kt->first = first;
                }
                current = findNodeInLevel(&current, word[i]);
                kt->totalKeys++;
            } else {
                current = found;
            }
        }
        prev = current;
        current = current->down;
        i++;
    }
    prev->end = 1;
    prev->word = strDup(word);
    kt->totalWords++;
}

void keysPredictRemoveWord(struct keysPredict* kt, char* word) {
    if (strLen(word) == 0){
        return;
    }
    struct node* current = kt->first;
    int i = 0;
    while (i < strLen(word)-1) {
        current = findNodeInLevel(&current, word[i]);
        if (current == NULL){
            return;
        }
        current = current->down;
        i++;
    }
    current = findNodeInLevel(&current, word[i]);
    if (current == NULL){
        return;
    }
    free(current->word);
    current->word = NULL;
    current->end = 0;
    kt->totalWords--;
}

struct node* keysPredictFind(struct keysPredict* kt, char* word) {
    int i = 0;
    struct node* curr = kt->first;
    while (i < strLen(word)-1) {
        curr = findNodeInLevel(&curr, word[i]);
        if (curr == NULL) {
            return NULL;
        }
        curr = curr->down;
        i++;
    }
    curr = findNodeInLevel(&curr, word[i]);
    if (curr == NULL) {
        return NULL;
    }
    if (curr->end == 1 && strCmp(curr->word, word)) {
        return curr;
    }
    return 0;
}

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {
    if (kt->first == NULL) {
        return NULL;
    }
    *wordsCount = 0;
    int i = 0;
    struct node* curr = kt->first;
    //PARARME AL FINAL DEL PREFIJO
    while (i < strLen(partialWord)) {
        curr = findNodeInLevel(&curr, partialWord[i]);
        if (curr == NULL) {
            return NULL;
        }
        curr = curr->down;
        i++;
    }
    if (curr == NULL) {
        return NULL;
    }
    //BUSCAR LAS PALABRAS RECURSIVAMENTE
    struct node* found = NULL;
    struct node* recorre = NULL;
    encontrarPalabras(curr, wordsCount, &found, &recorre);
    char** words = makeArrayFromList(found, *wordsCount);
    //LIBERAR MEMORIA DE LISTA AUXILIAR
    while(found != NULL) {
        struct node* temp = found;
        found = found->next;
        free(temp);
    }
    return words;
}
/*
int keysPredictCountWordAux(struct node* n) {

    // NO LA USE
}
*/

char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {
    char** words = keysPredictRun(kt, "", wordsCount);
    return words;
}

void keysPredictDelete(struct keysPredict* kt) {
    if (kt->first == NULL) {
        return;
    }
    struct node* current = kt->first;
    borrarRecursiva(current);    
    free(kt);
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

void printWords(char** words, int wordsCount) {
    printf("Palabras encontradas (%d):\n", wordsCount);
    for (int i = 0; i < wordsCount; i++) {
        printf("  %s\n", words[i]);
    }
}

struct node* findNodeInLevel(struct node** list, char character) {
    struct node* curr = *list;
    while (curr != NULL && curr->character != character) {
        curr = curr->next;
    }

    return curr;
}

struct node* addSortedNewNodeInLevel(struct node** list, char character) {
    struct node* curr = *list;
    struct node* prev = 0;
    struct node* nuevo = (struct node*)malloc(sizeof(struct node));
    nuevo->character = character;
    nuevo->end = 0;
    nuevo->word = 0;
    nuevo->down = NULL;
    while (curr != NULL && curr->character < character) {
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
    return *list;
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

void encontrarPalabras(struct node* curr, int* wordsCount, struct node** found, struct node** found_index){\
    if (curr == NULL) {
        return;
    }
    if (curr->down == NULL && curr->next == NULL) {
        if (curr->end == 1) {
            struct node* new = (struct node*)malloc(sizeof(struct node));
            new->character = curr->character;
            new->end = curr->end;
            new->word = curr->word;
            new->down = NULL;
            new->next = NULL;
            if (*found == NULL) {
                *found = new;
                *found_index = new;
            }else{
                (*found_index)->next = new;
                *found_index = new;
            }
            (*wordsCount)++;
        }
    }else{
        encontrarPalabras(curr->down, wordsCount, found, found_index);
        encontrarPalabras(curr->next, wordsCount, found, found_index);
        if (curr->end == 1){
            struct node* new = (struct node*)malloc(sizeof(struct node));
            new->character = curr->character;
            new->end = curr->end;
            new->word = curr->word;
            new->down = NULL;
            new->next = NULL;
            if (*found == NULL) {
                *found = new;
            }else{
                (*found_index)->next = new;
                *found_index = new;
            }
            (*wordsCount)++;
        }
    }
}

char** makeArrayFromList(struct node* found, int wordsCount){
    char** words = (char**)malloc(wordsCount * sizeof(char*));
    struct node* current = found;
    int i = 0;
    while (i < wordsCount) {
        words[i] = strDup(current->word);
        current = current->next;
        i++;
    }
    return words;
}

void borrarRecursiva(struct node* n){
    if (n == NULL){
        return;
    }
    if(n->down == NULL && n->next == NULL){
        if (n->end == 1){
            free(n->word);
        }
        free(n);
    }else{
        borrarRecursiva(n->down);
        borrarRecursiva(n->next);
        if (n->end == 1){
            free(n->word);
        }
        free(n);
    }
}
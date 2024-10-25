#include "utils.h"

int strLen(char* src) {
    int len = 0;
    while(src[len] != '\0') {
        len++;
    }
    return len;
}

char* strDup(char* src) {
    if (src == NULL) {
        return NULL;
    }
    char* rv = (char*)malloc(sizeof(char)*(strLen(src)+1));
    for(int i = 0; i < strLen(src)+1; i++) {
        rv[i] = src[i];
    }
    return rv;
}

//esta funcion no la pedian pero la hice para comparar strings
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
    if (prev->end!=1){
        prev->end = 1;
        prev->word = strDup(word);
        kt->totalWords++;
    }
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

//versión recursiva, la que más me gusta
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
    struct node* lista_palabras = NULL;
    struct node* palabras_recorre = NULL;
    encontrarPalabras(curr, wordsCount, &lista_palabras, &palabras_recorre);
    char** words = makeArrayFromList(lista_palabras, *wordsCount);
    //LIBERAR MEMORIA DE LISTA AUXILIAR
    while(lista_palabras != NULL) {
        struct node* temp = lista_palabras;
        lista_palabras = lista_palabras->next;
        free(temp);
    }
    return words;
}

//versión iterativa, simil BFS y que usa nuevo tipo de datos impostor_node
char** keysPredictRun_v2(struct keysPredict* kt, char* partialWord, int* wordsCount){
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

    //RECORRER RESTO DEL ARBOL Y GUARDAR PALABRAS EN LISTA AUXILIAR
    struct impostor_node* lista = impostorFromNode(curr); //especie de queue de nodos que voy descubriendo
    struct impostor_node* last = lista; //para no perder la referencia al final de la lista
    struct impostor_node* v = lista; //para recorrer la lista
    struct impostor_node* palabras_list = NULL; //lista de palabras que voy encontrando y transformaré a array
    struct impostor_node* palabras_recorre = NULL; //para no perder la referencia al final de la lista
    int k = 0;
    while(v != NULL){
        if (v->end == 1){
            (*wordsCount)++;
            if(palabras_list == NULL){
                palabras_list = impostorDup(v);
                palabras_recorre = palabras_list;
            }else{
                palabras_recorre->fake_next = impostorDup(v);
                palabras_recorre = palabras_recorre->fake_next;
            }
        }
        if (v->next != NULL){
            last->fake_next = impostorFromNode(v->next);
            last = last->fake_next;
        }
        if (v->down != NULL){
            last->fake_next = impostorFromNode(v->down);
            last = last->fake_next;
        }
        v = v->fake_next;
        k++;
    }

    //GENERAR VR LIBERAR MEMORIA DE ESTRUCTURAS AUXILIARES
    char** words = makeArrayFromImpostorList(palabras_list, *wordsCount);
    while(palabras_list != NULL) {
        struct impostor_node* temp = palabras_list;
        palabras_list = palabras_list->fake_next;
        free(temp->word);
        free(temp);
    }
    while(lista != NULL) {
        struct impostor_node* temp = lista;
        lista = lista->fake_next;
        free(temp->word);
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

//ACA EMPIEZAN LAS FUNCIONES QUE NO ESTÁN EN EL ENUNCIADO
// Función para imprimir una lista de nodos (para debug)
void printList(struct node** list) {
    struct node* current = *list;
    while (current != NULL) {

        printf("%c, ", current->character);
        current = current->next;
    }
    printf("\n");
}

// Función para imprimir un array de palabras (para debug)
void printWords(char** words, int wordsCount) {
    printf("Palabras encontradas (%d):\n", wordsCount);
    for (int i = 0; i < wordsCount; i++) {
        printf("  %s\n", words[i]);
    }
}

// Función recursiva para encontrar palabras (para keysPredictRun)
void encontrarPalabras(struct node* curr, int* wordsCount, struct node** lista_palabras, struct node** palabras_recorre){\
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
            if (*lista_palabras == NULL) {
                *lista_palabras = new;
                *palabras_recorre = new;
            }else{
                (*palabras_recorre)->next = new;
                *palabras_recorre = new;
            }
            (*wordsCount)++;
        }
    }else{
        encontrarPalabras(curr->down, wordsCount, lista_palabras, palabras_recorre);
        encontrarPalabras(curr->next, wordsCount, lista_palabras, palabras_recorre);
        if (curr->end == 1){
            struct node* new = (struct node*)malloc(sizeof(struct node));
            new->character = curr->character;
            new->end = curr->end;
            new->word = curr->word;
            new->down = NULL;
            new->next = NULL;
            if (*lista_palabras == NULL) {
                *lista_palabras = new;
            }else{
                (*palabras_recorre)->next = new;
                *palabras_recorre = new;
            }
            (*wordsCount)++;
        }
    }
}

// Función para convertir una lista de nodos en un array de strings (para keysPredictRun)
char** makeArrayFromList(struct node* lista_palabras, int wordsCount){
    char** words = (char**)malloc(wordsCount * sizeof(char*));
    struct node* current = lista_palabras;
    int i = 0;
    while (i < wordsCount) {
        words[i] = strDup(current->word);
        current = current->next;
        i++;
    }
    return words;
}

// Función para convertir una lista de impostor_nodes en un array de strings (para keysPredictRun_v2)
char** makeArrayFromImpostorList(struct impostor_node* lista_palabras, int wordsCount){
    char** words = (char**)malloc(wordsCount * sizeof(char*));
    struct impostor_node* current = lista_palabras;
    int i = 0;
    while (i < wordsCount) {
        words[i] = strDup(current->word);
        current = current->fake_next;
        i++;
    }
    return words;
}

// duplicar un impostor_node (para keysPredictRun_v2)
struct impostor_node* impostorDup(struct impostor_node* n){
    struct impostor_node* rv = (struct impostor_node*) malloc(sizeof(struct impostor_node));
    rv->character = n->character;
    rv->end = n->end;
    rv->word = strDup(n->word);
    rv->down = n->down;
    rv->next = n->next;
    rv->fake_next = NULL;
    return rv;
}

// crear un impostor_node a partir de un node (para keysPredictRun_v2)
struct impostor_node* impostorFromNode(struct node* n){
    struct impostor_node* rv = (struct impostor_node*) malloc(sizeof(struct impostor_node));
    rv->character = n->character;
    rv->end = n->end;
    rv->word = strDup(n->word);
    rv->down = n->down;
    rv->next = n->next;
    rv->fake_next = NULL;
    return rv;
}

// borrar un arbol de nodos recursivamente (para keysPredictDelete)
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
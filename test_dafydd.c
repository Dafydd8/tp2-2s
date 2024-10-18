#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

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
    struct node* first = addSortedNewNodeInLevel(nodeList, 'a');
    first = addSortedNewNodeInLevel(nodeList, 'c');
    first = addSortedNewNodeInLevel(nodeList, 'm');
    printList(nodeList);
    while(first != NULL) {
        struct node* temp = first;
        first = first->next;
        free(temp);
    }


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


    // Pruebas para keysPredictAddWord (Ejercicio 3)
    printf("\nTest keysPredictAddWord:\n");

    // Crear nueva estructura keysPredict
    struct keysPredict* kt = keysPredictNew();

    // Caso 1: Agregar una palabra simple
    printf("Caso 1: Agregar la palabra 'casa'.\n");
    keysPredictAddWord(kt, "casa");
    keysPredictPrint(kt);
    // Caso 2: Agregar una palabra que comparte prefijo con la anterior
    printf("\nCaso 2: Agregar la palabra 'capa' (comparte prefijo con 'casa').\n");
    keysPredictAddWord(kt, "capa");
    keysPredictPrint(kt);

    // Caso 3: Agregar una palabra totalmente nueva
    printf("\nCaso 3: Agregar la palabra 'perro'.\n");
    keysPredictAddWord(kt, "perro");
    keysPredictPrint(kt);

    // Caso 4: Agregar una palabra que es un prefijo de otra palabra
    printf("\nCaso 4: Agregar la palabra 'ca' (es prefijo de 'casa' y 'capa').\n");
    keysPredictAddWord(kt, "ca");
    keysPredictPrint(kt);

    // Caso 5: Agregar una palabra con letras repetidas
    printf("\nCaso 5: Agregar la palabra 'llama'.\n");
    keysPredictAddWord(kt, "llama");
    keysPredictPrint(kt);

    // Pruebas para keysPredictRemoveWord (Ejercicio 3)
    printf("\nTest keysPredictRemoveWord:\n");
    keysPredictRemoveWord(kt, "casa");
    keysPredictPrint(kt);


    // Pruebas para keysPredictFind (Ejercicio 3)
    printf("\nTest keysPredictFind:\n");

    // Caso 1: Buscar una palabra que existe
    printf("Caso 1: Buscar la palabra 'capa'.\n");
    found = keysPredictFind(kt, "capa");
    if (found) {
        printf("Palabra 'casa' encontrada.\n");
    } else {
        printf("Palabra 'casa' no encontrada.\n");
    }
    // Caso 1.5: Buscar una palabra que existía y ya no
    printf("\nCaso 1.5: Buscar la palabra 'casa (existía y ya no más)'.\n");
    found = keysPredictFind(kt, "casa");
    if (found) {
        printf("Palabra 'casa' encontrada.\n");
    } else {
        printf("Palabra 'casa' no encontrada.\n");
    }

    // Caso 2: Buscar una palabra que no existe
    printf("\nCaso 2: Buscar la palabra 'gato' (no existe).\n");
    found = keysPredictFind(kt, "gato");
    if (found) {
        printf("Palabra 'gato' encontrada.\n");
    } else {
        printf("Palabra 'gato' no encontrada.\n");
    }

    // Caso 3: Buscar una palabra que es un prefijo de otras pero no es palabra completa
    printf("\nCaso 3: Buscar la palabra 'ca' (prefijo de 'casa' y 'capa').\n");
    found = keysPredictFind(kt, "ca");
    if (found) {
        printf("Palabra 'ca' encontrada.\n");
    } else {
        printf("Palabra 'ca' no encontrada.\n");
    }

    // Caso 4: Buscar una palabra que comparte letras con otra pero no está completa
    printf("\nCaso 4: Buscar la palabra 'cap' (parcial de 'capa').\n");
    found = keysPredictFind(kt, "cap");
    if (found) {
        printf("Palabra 'cap' encontrada.\n");
    } else {
        printf("Palabra 'cap' no encontrada.\n");
    }

    // Caso 5: Buscar una palabra con letras repetidas
    printf("\nCaso 5: Buscar la palabra 'llama'.\n");
    found = keysPredictFind(kt, "llama");
    if (found) {
        printf("Palabra 'llama' encontrada.\n");
    } else {
        printf("Palabra 'llama' no encontrada.\n");
    }

    keysPredictDelete(kt);
    // Pruebas para keysPredictRun (Ejercicio 3)
    printf("\nTest keysPredictRun:\n");

    // Crear nueva estructura keysPredict
    kt = keysPredictNew();

    // Agregar algunas palabras a la estructura
    keysPredictAddWord(kt, "casa");
    keysPredictAddWord(kt, "capa");
    keysPredictAddWord(kt, "carro");
    keysPredictAddWord(kt, "cargar");
    keysPredictAddWord(kt, "perro");
    keysPredictAddWord(kt, "per");
    keysPredictAddWord(kt, "pelota");
    keysPredictAddWord(kt, "llama");
    keysPredictPrint(kt);
    int wordsCount = 0;
    words = NULL;

    // Caso 1: Prefijo que coincide con varias palabras
    printf("Caso 1: Prefijo 'ca' (debería encontrar 'casa', 'capa', 'carro', 'cargar').\n");
    words = keysPredictRun_v2(kt, "ca", &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);
    // Caso 2: Prefijo que coincide con una sola palabra
    printf("\nCaso 2: Prefijo 'pel' (debería encontrar 'pelota').\n");
    wordsCount = 0;
    words = keysPredictRun(kt, "pel", &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);

    // Caso 3: Prefijo que es una palabra completa y tiene palabras adicionales
    printf("\nCaso 3: Prefijo 'per' (debería encontrar 'perro').\n");
    wordsCount = 0;
    words = keysPredictRun(kt, "per", &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);

    // Caso 4: Prefijo que no coincide con ninguna palabra
    printf("\nCaso 4: Prefijo 'gat' (no debería encontrar ninguna palabra).\n");
    wordsCount = 0;
    words = keysPredictRun_v2(kt, "gat", &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);

    // Caso 5: Prefijo vacío (debería encontrar todas las palabras)
    printf("\nCaso 5: Prefijo vacío (debería encontrar todas las palabras).\n");
    wordsCount = 0;
    words = keysPredictRun(kt, "", &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);


    // Pruebas para keysPredictListAll (Ejercicio 3)
    printf("\nTest keysPredictListAll:\n");

    wordsCount = 0;
    words = NULL;

    // Caso 1: Listar todas las palabras almacenadas
    printf("Caso 1: Listar todas las palabras.\n");
    words = keysPredictListAll(kt, &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);

    // Caso 2: Listar palabras después de eliminar algunas
    printf("\nCaso 2: Eliminar 'cargar' y listar todas las palabras.\n");
    keysPredictRemoveWord(kt, "cargar");
    wordsCount = 0;
    words = keysPredictListAll(kt, &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);

    // Caso 3: Listar palabras en una estructura vacía
    printf("\nCaso 3: Eliminar todas las palabras y listar.\n");
    keysPredictRemoveWord(kt, "casa");
    keysPredictRemoveWord(kt, "capa");
    keysPredictRemoveWord(kt, "carro");
    keysPredictRemoveWord(kt, "perro");
    keysPredictRemoveWord(kt, "pelota");
    keysPredictRemoveWord(kt, "llama");
    keysPredictRemoveWord(kt, "per");
    wordsCount = 0;
    words = keysPredictListAll(kt, &wordsCount);
    printWords(words, wordsCount);
    deleteArrayOfWords(words, wordsCount);

    // Liberar la memoria usada por la estructura
    keysPredictDelete(kt);

    printf("Done!\n");

    return 0;
}
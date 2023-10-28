/*******************************
            Include
 *******************************/
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/*******************************
            Define
 *******************************/
#define MAX_COMMAND_DIM 20
#define INIT_VEHICLES 512
#define INF INT_MAX

/*******************************
      Strutture per Veicoli
 *******************************/

typedef struct Vehicle_s{
    int autonomie[INIT_VEHICLES];
    int size;
    int maxAutonomia;
} Vehicle_t;

/*******************************
      Strutture per Albero
 *******************************/
typedef struct RBnode_s{   // Struttura di un nodo per RB TREE
    int distanza;
    Vehicle_t * veicoli;
    struct RBnode_s * left;
    struct RBnode_s * right;
    struct RBnode_s * father;
    int color; // 0 per rosso , 1 per nero !
    char status;
}RBnode_t;

typedef struct RBTree_s{
    struct RBnode_s * root; // attenzione qui
}RBTree_t ;

static RBnode_t * nill;


/*******************************
        Strutture per Pila
 *******************************/

typedef struct Station_s{
    RBnode_t * stazione;
    struct Station_s * next;
    struct Station_s * prev;
    struct Station_s * backtrack;
} Station_t;


typedef struct Node_s {
    Station_t * data;
    struct Node_s *next;
} Node_t;

typedef struct Stack_s{
    Node_t *top;
} Stack_t;


/*******************************
        Funzioni per Pila
 *******************************/

// Funzione per creare una nuova pila
Stack_t *createStack() {
    Stack_t *stack = (Stack_t *)malloc(sizeof(Stack_t));
    stack->top = NULL;
    return stack;
}

// Funzione per verificare se la pila è vuota
int isEmpty(Stack_t *stack) {
    return stack->top == NULL;
}

// Funzione per inserire un elemento nella pila
void push(Stack_t *stack, Station_t * value) {
    Node_t *newNode = (Node_t *)malloc(sizeof(Node_t));
    newNode->data = value;
    newNode->next = (stack->top);
    stack->top = newNode;
}

// Funzione per rimuovere un elemento dalla pila
Station_t * pop(Stack_t *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty. Cannot pop.\n");
        return NULL;
    }
    Node_t *temp = stack->top;
    Station_t * poppedValue = temp->data;
    stack->top = temp->next;
    free(temp);
    return poppedValue;
}

// Funzione per ottenere il valore in cima alla pila senza rimuoverlo
Station_t *top(Stack_t *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty.\n");
        return NULL;
    }
    return stack->top->data;
}

// Funzione per liberare la memoria allocata per la pila
void freeStack(Stack_t *stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

void printPercorso(Stack_t * stack, int arrivo){

    Station_t *toPrint = NULL;
    toPrint = pop(stack);

    printf("%d ", toPrint->stazione->distanza);

    while( !isEmpty(stack) ){
        toPrint = pop(stack);
        if(toPrint->stazione->distanza == arrivo){
            printf("%d\n", arrivo);
            break ;
        }
        printf("%d ", toPrint->stazione->distanza);
    }
}



/*******************************
        Funzioni per Veicoli
 *******************************/

Vehicle_t *createDynamicArray() {
    Vehicle_t *array = (Vehicle_t *)malloc(sizeof(Vehicle_t));
    array->size = 0;
    array->maxAutonomia = 0;
    for(int i = 0; i < INIT_VEHICLES; i++){array->autonomie[i] = 0;}
    return array;
}

void pushBack(Vehicle_t *array, int value) {

    int dim = array->size;
    array->autonomie[dim] = value;
    array->size = array->size + 1;
}


void removeElement(Vehicle_t *array, int index) {
    if (index >= array->size) {
        printf("non rottamata\n");
        return;
    }

    // Sposta gli elementi successivi indietro di una posizione
    for (int i = index; i < array->size - 1; i++) {
        array->autonomie[i] = array->autonomie[i + 1];
    }

    array->size--;
    printf("rottamata\n");
}

void removeElementByValue(Vehicle_t *array, int value) {
    for (int i = 0; i < array->size; i++) {
        if (array->autonomie[i] == value) {
            removeElement(array, i);
            return;
        }
    }

    printf("non rottamata\n");
}

int findIndexByValue(Vehicle_t *array, int value) {
    for (int i = 0; i < array->size; i++) {
        if (array->autonomie[i] == value) {
            return i; // Restituisce l'indice se il valore è stato trovato
        }
    }
    return -1; // Restituisce -1 se il valore non è stato trovato
}

int findMaxElementV(Vehicle_t *array) {
    int dim = array->size;
    if (dim == 0) {
        // Array Vuoto
        return -1;
    }

    int max = array->autonomie[0];
    for (int i = 1; i < dim; i++) {
        if (array->autonomie[i] > max) {
            max = array->autonomie[i];
        }
    }
    return max;
}

void freeDynamicArray(Vehicle_t *array) {
    array->size = 0;
    array->maxAutonomia = 0;
    free(array);
    //free(array->autonomie);
    //free(array);
}

/*******************************
        Funzioni per Albero
 *******************************/

RBnode_t * costruisciStazione(int newData, int autonomie[], int numAuto){  // Materializzazione del nodo
    RBnode_t * newNode;
    newNode = malloc(sizeof(RBnode_t) );

    if (newNode == NULL) {
        perror("Errore nell'allocazione di memoria");
        exit(EXIT_FAILURE);
    }

    newNode->left = nill;
    newNode->right = nill;
    newNode->father = nill;
    newNode->color = 1;  // 1 NERO, 0 ROSSO

    newNode->distanza = newData;
    newNode->status = 'f';
    newNode->veicoli = createDynamicArray();

    for(int i = 0; i < numAuto; i++){
        pushBack( (newNode->veicoli), autonomie[i]);
        if(autonomie[i] > newNode->veicoli->maxAutonomia) newNode->veicoli->maxAutonomia = autonomie[i];
    }
    return newNode;
}

RBnode_t * searchElement(RBnode_t * root, int key) {

    if ( root == nill )
        return nill;

    if (root->distanza == key) {
        return root;
    }

    if (key < root->distanza) {
        return searchElement(root->left, key);
    } else {
        return searchElement(root->right, key);
    }
}

void LeftRotateRB(RBTree_t * RootTree, RBnode_t * x) {
    if (x -> right == nill)
        return;

    RBnode_t * y = x -> right;

    x -> right = y -> left;
    if (y -> left != nill)
        y -> left -> father = x;
    y -> father = x -> father;
    if (x -> father == nill)
        RootTree -> root = y;
    else if (x == x -> father -> left)
        x -> father -> left = y;
    else
        x -> father -> right = y;
    y -> left = x;
    x -> father = y;

    return;
}

// Per il RightRotate copio la LeftRotate e vado a sostituire opportunamente i puntatori a right con left !
void RightRotateRB(RBTree_t * RootTree, RBnode_t * x) {
    if (x -> left == nill)
        return;

    RBnode_t * y = x -> left;

    x -> left = y -> right;
    if (y -> right != nill)
        y -> right -> father = x;
    y -> father = x -> father;
    if (x -> father == nill)
        RootTree -> root = y;
    else if (x == x -> father -> right)
        x -> father -> right = y;
    else
        x -> father -> left = y;
    y -> right = x;
    x -> father = y;

    return;
}


void InsertFixUpRB(RBTree_t * tree, RBnode_t * z) {
    RBnode_t * x;
    RBnode_t * y;

    if (z == tree -> root) {
        (tree->root)->color = 1;
    }else {
        x = z -> father;
        if (x -> color == 0) {
            if (x == x -> father -> left) {
                y = x -> father -> right;
                if (y -> color == 0) {
                    x -> color = 1;
                    y -> color = 1;
                    x -> father -> color = 0;
                    InsertFixUpRB(tree, x->father);
                } else {
                    if (z == x -> right) {
                        z = x;
                        LeftRotateRB(tree,z);
                        x = z -> father;
                    }
                    x -> color = 1;
                    x -> father -> color = 0;
                    RightRotateRB(tree,x -> father);
                }
            } else {
                y = x -> father -> left;
                if (y -> color == 0) {
                    x -> color = 1;
                    y -> color = 1;
                    x -> father -> color = 0;
                    InsertFixUpRB(tree, x->father);
                } else {
                    if (z == x -> left) {
                        z = x;
                        RightRotateRB(tree,z);
                        x = z -> father;
                    }
                    x -> color = 1;
                    x -> father -> color = 0;
                    LeftRotateRB(tree,x -> father);
                }
            }
        }
    }

    return;
}


void * inserisciStazione(RBTree_t * tree, int newData, int autonomie[], int numAuto) {
    RBnode_t * x = tree -> root;
    RBnode_t * y = nill;
    RBnode_t * z;
    RBnode_t * helper;

    helper = searchElement(x, newData);
    if(helper != nill && helper->distanza == newData){
        printf("non aggiunta\n");
        return nill;
    }

    while (x != nill) {
        y = x;
        if (newData == x -> distanza)
            return x;
        if (newData < x -> distanza)
            x = x -> left;
        else
            x = x -> right;
    }
    z = costruisciStazione(newData, autonomie, numAuto);
    printf("aggiunta\n");

    z -> father = y;
    if (y == nill) {
        tree->root = z;
    }else {
        if ( (z->distanza < y->distanza) )
            y->left = z;
        else
            y->right = z;
    }

    z -> left = nill;
    z -> right = nill;
    z -> color = 0;

    // printf("Ho inserito chiave con padre pre fix %s\n", (aux->contenuto) );

    InsertFixUpRB(tree, z);

    //aux = z->father;

    //printf("Ho inserito chiave con padre %s\n", (aux->contenuto) );

    return z;
}

RBnode_t * searchMinimum(RBnode_t * nodo_aux) {
    while(nodo_aux -> left != nill) {
        nodo_aux = nodo_aux -> left;
    }

    return nodo_aux;
}

RBnode_t *searchMaximum(RBnode_t * nodo_aux) {
    while (nodo_aux->right != nill) {
        nodo_aux = nodo_aux->right;
    }
    return nodo_aux;
}

RBnode_t *findPredecessor(RBnode_t *x) {
    RBnode_t * y;

    if (x -> left != nill)
        return searchMaximum(x -> left); //
    y = x -> father;
    while ((y != nill) && (x == y -> left)) {
        x = y;
        y = y -> father;
    }

    return y;
}

RBnode_t * findSuccessor(RBnode_t * x) {
    RBnode_t * y;

    if (x -> right != nill)
        return searchMinimum(x -> right); // Implementare altra searchMinimum
    y = x -> father;
    while ((y != nill) && (x == y -> right)) {
        x = y;
        y = y -> father;
    }

    return y;
}


void DeleteFixUpRB(RBTree_t * tree, RBnode_t * x) {
    RBnode_t * w;

    if ((x->color == 0) || (x->father == nill))
        x->color = 1;
    else if (x == x->father->left) {
        w = x->father->right;
        if (w->color == 0) {
            w->color = 1;
            x->father->color = 0;
            LeftRotateRB(tree, x->father);
            w = x->father->right;
        }
        if ((w->left->color == 1) && (w->right->color == 1)) {
            w->color = 0;
            DeleteFixUpRB(tree, x->father);
        } else {
            if (w->right->color == 1) {
                w->left->color = 1;
                w->color = 0;
                RightRotateRB(tree, w);
                w = x->father->right;
            }
            w->color = x->father->color;
            x->father->color = 1;
            w->right->color = 1;
            LeftRotateRB(tree, x->father);
        }
    } else {
        w = x->father->left;
        if (w->color == 0) {
            w->color = 1;
            x->father->color = 0;
            RightRotateRB(tree, x->father);
            w = x->father->left;
        }
        if ((w->right->color == 1) && (w->left->color == 1)) {
            w->color = 0;
            DeleteFixUpRB(tree, x->father);
        } else {
            if (w->left->color == 1) {
                w->right->color = 1;
                w->color = 0;
                LeftRotateRB(tree, w);
                w = x->father->left;
            }
            w->color = x->father->color;
            x->father->color = 1;
            w->left->color = 1;
            RightRotateRB(tree, x->father);
        }
    }

    return;
}



// Lo cancella dall'albero, poi bisogna fare free nel contesto giusto !
void  rimuoviStazione(RBTree_t * tree, int key) {
    RBnode_t * x;
    RBnode_t * y;
    RBnode_t * z = searchElement(tree -> root, key);  // Implementare altra search
    int  p;
    Vehicle_t * p_veic;

    if (z == nill){
        printf("non demolita\n");
        return ;
    }

    if ((z -> left == nill) || (z -> right == nill))
        y = z;
    else y = findSuccessor(z); // implementare altra min
    if (y -> left != nill)
        x = y -> left;
    else x = y -> right;
    x -> father = y -> father;
    if (y -> father == nill)
        tree -> root = x;
    else if (y == y -> father -> left)
        y -> father -> left = x;
    else
        y -> father -> right = x;
    if (y != z) {
        p = z -> distanza;
        p_veic = z->veicoli;

        z -> distanza = y -> distanza;
        z->veicoli = y->veicoli;

        y -> distanza = p;
        y->veicoli = p_veic;

    }
    if (y -> color  == 1)
        DeleteFixUpRB(tree, x);

    // free di y
    //freeDynamicArray((y->veicoli));
    free(y);
    printf("demolita\n");
    //return y;
}


void svuotaAlbero(RBnode_t * nodo_aux) {
    if(nodo_aux != nill) {
        svuotaAlbero(nodo_aux -> left);
        svuotaAlbero(nodo_aux -> right);

        free(nodo_aux);
    }
}

/*******************************
        Funzioni per Auto
 *******************************/

void aggiungiAuto(RBTree_t * h, int distanza, int autonomia){

    RBnode_t * stazione = (RBnode_t *) searchElement(h->root, distanza);

    if(stazione != nill){
        if(autonomia > stazione->veicoli->maxAutonomia) stazione->veicoli->maxAutonomia = autonomia;
        pushBack((stazione->veicoli), autonomia);
        printf("aggiunta\n");
    } else printf("non aggiunta\n");

    return ;
}

void modAggiungiAuto(RBnode_t * h, int autonomia){

    RBnode_t * stazione = h;
    Vehicle_t * toCheckVeicolo = stazione->veicoli;

    if(stazione != nill){
        if(autonomia > toCheckVeicolo->maxAutonomia) toCheckVeicolo->maxAutonomia = autonomia;
        pushBack((toCheckVeicolo), autonomia);
        printf("aggiunta\n");
    } else printf("non aggiunta\n");

    return ;

}

void rottamaAuto(RBTree_t * h, int distanza, int autonomia){

    RBnode_t * stazione = (RBnode_t *) searchElement(h->root, distanza);
    Vehicle_t * tooLookAuto = stazione->veicoli;

    if(stazione != nill){
        int parcheggiate = tooLookAuto->size;
        int foundIndex = findIndexByValue((tooLookAuto),autonomia);
        if(foundIndex == -1){
            printf("non rottamata\n");
            return ;
        }
        if (foundIndex < parcheggiate) {
            printf("rottamata\n");
            // Sposta gli elementi successivi indietro per sovrascrivere l'elemento da rimuovere
            for (int i = foundIndex; i < parcheggiate; i++) {
                tooLookAuto->autonomie[i] = tooLookAuto->autonomie[i + 1];
            }
            tooLookAuto->size--; // Decrementa la dimensione dell'array per indicare la rimozione
            tooLookAuto->maxAutonomia = findMaxElementV((tooLookAuto));

        } else if(foundIndex == parcheggiate){
            tooLookAuto->autonomie[parcheggiate] = 0;
            tooLookAuto->size--; // Decrementa la dimensione dell'array per indicare la rimozione
            tooLookAuto->maxAutonomia = findMaxElementV((tooLookAuto));
            printf("rottamata\n");

        } else printf("non rottamata\n");
    } else printf("non rottamata\n");
}



/*******************************
        Funzioni per Lista
            Temporanea
 *******************************/


// Funzione per creare un nuovo elemento
Station_t  *creaElementoStaz(RBnode_t * station) {
    Station_t *nuovoElemento = ( Station_t *)malloc(sizeof( Station_t));
    if (nuovoElemento == NULL) {
        fprintf(stderr, "Errore: memoria esaurita.\n");
        exit(1);
    }
    nuovoElemento->stazione = station;
    nuovoElemento->next = NULL;
    nuovoElemento->prev = NULL;
    return nuovoElemento;
}

// Funzione per aggiungere un elemento in testa alla lista
Station_t *aggiungiInTestaStaz(Station_t *testa, RBnode_t * station) {
    Station_t *nuovoElemento = creaElementoStaz(station);
    nuovoElemento->next = testa;
    if (testa != NULL) {
        testa->prev = nuovoElemento;
    }
    return nuovoElemento;
}

// Funzione per deallocare la memoria di un elemento
void liberaElementoStaz(Station_t *elemento) {
    if (elemento != NULL) {// Libera la memoria associata al campo "valore"
        free(elemento);
    }
}

// Funzione per deallocare l'intera lista
void liberaLista(Station_t *testa) {
    Station_t * temp;
    while (testa != NULL) {
        temp = testa;
        testa = testa->next;
        free(temp); // Libera l'elemento corrente
    }
}

Station_t *creaLista(RBTree_t h, int partenza, int arrivo){

    int aux;
    RBnode_t * toAdd = nill;
    Station_t * stazioni = NULL;

    if(arrivo < partenza){
        aux = arrivo;
        arrivo = partenza;
        partenza = aux;
    }

    toAdd = searchElement(h.root, arrivo);

    while(toAdd != nill && toAdd->distanza > partenza){
        stazioni = aggiungiInTestaStaz(stazioni, toAdd);

        toAdd = findPredecessor(toAdd);

    }
    if(toAdd->distanza == partenza){
        stazioni = aggiungiInTestaStaz(stazioni, toAdd);
    }
    return stazioni;
}

// Funzione per cercare un elemento nella lista per nome
Station_t *trovaElemento(Station_t *testa, int target) {
    Station_t *corrente = testa;

    while (corrente != NULL) {
        if (corrente->stazione->distanza == target) {
            return corrente; // Elemento trovato, restituisci il puntatore
        }
        corrente = corrente->next;
    }

    return NULL; // Elemento non trovato, restituisci NULL
}




/*******************************
          Funzioni per
         Stampare Percorso
 *******************************/


void stampaPercorso(Station_t * startNode, int target){

    Station_t * current = startNode;

    while (current != NULL) {

        if(current->stazione->distanza == target) {
            if(current->stazione->status == 't') {
                printf("%d\n", current->stazione->distanza);
                current->stazione->status = 'f';
            }
            return ;
        }

        if(current->stazione->status == 't') {
            printf("%d ", current->stazione->distanza);
            current->stazione->status = 'f';
        }
        current = current->next;
    }

    return ;  // Ritorna NULL se l'elemento non è stato trovato
}


void AltprintTraceback_v2(Station_t * testina, int partenza, int arrivo){

    Station_t * helper = NULL;
    Station_t * aux = testina;
    Stack_t * percorso = createStack();


    while (aux != NULL && aux->stazione != nill && aux->stazione->distanza < partenza){
        push(percorso, aux);
        helper = aux->backtrack;
        aux->backtrack = NULL;
        // PUSHO AUX E NON AUX STAZIONE
        aux = helper;
    }

    /* while (testina != NULL && testina->distanza > partenza){
         printf("%d ", testina->distanza);
         helper = testina->backtrack;
         testina->backtrack = NULL;
         testina = helper;
     }*/

    if(aux != NULL && aux->stazione->distanza == partenza){
        push(percorso, aux);
        aux->backtrack = NULL;
    }

    Station_t * toPrint;

    while( !isEmpty(percorso) ){
        toPrint = pop(percorso);
        if(toPrint->stazione->distanza == arrivo){
            printf("%d\n", arrivo);
            break ;
        }
        printf("%d ", toPrint->stazione->distanza);
    }

    return ;
}


/*******************************
          Funzioni per
         Pulire Percorso
 *******************************/

void pulisciPercorso(Station_t * startNode, int partenza){

    Station_t * current = startNode;

    while (current != NULL) {
        if(current->stazione->status == 't') {
            current->stazione->status = 'f';
            if(current->stazione->distanza == partenza) {
                printf("\n");
                return ;
            }
        }
        current = current->prev;
    }

    return ;  // Ritorna NULL se l'elemento non è stato trovato
}



void clearTraceback(Station_t * testina, int arrivo){
    Station_t * auxil = testina;

    while (auxil != NULL && auxil->stazione->distanza >= arrivo){
        auxil->backtrack = NULL;
        auxil = auxil->prev;
    }

}



/*******************************
          Funzioni per
          Trova Percorso
 *******************************/




void BFSFindDistance(int partenza, int arrivo, Station_t * testina){

    // Corner Case
    if(partenza == arrivo){
        printf("%d\n", partenza);
        return ;
    }

    Station_t * helper_ext = trovaElemento(testina,partenza);
    Station_t * partenza_og = helper_ext;
    if(partenza - helper_ext->stazione->veicoli->maxAutonomia <= arrivo){
        printf("%d %d\n", partenza, arrivo);
        return ;
    }

    // Inizia ricerca BFS
    Stack_t *stack1 = createStack();
    Stack_t *stack2 = createStack();
    Station_t * helper = helper_ext;

    Station_t * min_distance = partenza_og;
    push(stack1, helper_ext);


    while( !isEmpty(stack1) || !isEmpty(stack2) ){

        if( !isEmpty(stack1) && isEmpty(stack2) ){

            if(top(stack1)->stazione->distanza == arrivo){
                AltprintTraceback_v2(top(stack1), partenza, arrivo);
                clearTraceback(partenza_og, arrivo);
                return ;
            }

            helper_ext = top(stack1);
            helper = helper_ext;
            while(!isEmpty(stack1)){
                while(helper != NULL && helper->stazione->distanza >= arrivo){
                    // Trova gli adiacenti al passo vicino
                    if(helper_ext->stazione->distanza - helper_ext->stazione->veicoli->maxAutonomia <= helper->stazione->distanza ){
                        if(helper->stazione->distanza < min_distance->stazione->distanza){
                            min_distance = helper;
                            helper->backtrack = helper_ext;
                            push(stack2, min_distance);
                            // AGG INDICE TRACEBACK
                            // altrimenti l'ho gia visto
                        }
                        helper = helper->prev;
                    } else {
                        break;
                    }
                }
                pop(stack1);
                if( !isEmpty(stack1) ){
                    helper_ext = top(stack1);
                    helper = helper_ext;
                }
            }


        } else if( !isEmpty(stack2) && isEmpty(stack1) ){

            if(top(stack2)->stazione->distanza == arrivo){
                AltprintTraceback_v2(top(stack2), partenza, arrivo);
                clearTraceback(partenza_og, arrivo);
                return ;
            }

            helper_ext = top(stack2);
            helper = helper_ext;
            while(!isEmpty(stack2)){
                while(helper != NULL && helper->stazione->distanza >= arrivo){
                    if(helper_ext->stazione->distanza - helper_ext->stazione->veicoli->maxAutonomia <= helper->stazione->distanza ){
                        if(helper->stazione->distanza < min_distance->stazione->distanza){
                            min_distance = helper;
                            helper->backtrack = helper_ext;
                            push(stack1, min_distance);
                            // AGG INDICE TRACEBACK
                            // altrimenti l'ho gia visto
                        }
                        helper = helper->prev;
                    } else {
                        break;
                    }
                }
                pop(stack2);
                if( !isEmpty(stack2) ){
                    helper_ext = top(stack2);
                    helper = helper_ext;
                }
            }


        }

    }

    printf("nessun percorso\n");
    clearTraceback(testina, arrivo);
    return ;

}

void newStampaPercorso(Stack_t * percorso, int arrivo){

    Station_t * toPrint;

    while( !isEmpty(percorso) ){
        toPrint = pop(percorso);
        if(toPrint->stazione->distanza == arrivo){
            printf("%d\n", arrivo);
            break ;
        }
        printf("%d ", toPrint->stazione->distanza);
    }

    return ;
}


void findDistance(int partenza, int arrivo, Station_t * testina){

    Station_t * helper_esterno;
    Station_t * helper;
    Station_t * toSave;
    Station_t  * lastSeen;
    //Station_t * stat_arriv;


    if(partenza == arrivo){
        printf("%d\n", partenza);
        return ;
    }

    helper_esterno = trovaElemento(testina, arrivo);
    int arrivo_og = arrivo;
    helper = helper_esterno;
    //stat_arriv = helper_esterno;
    lastSeen = NULL;
    toSave = NULL;


    Stack_t *stack1 = createStack();
    push(stack1, helper_esterno);

    //helper_esterno->stazione->status = 't';

    while (helper_esterno != NULL && helper_esterno->stazione->distanza >= partenza){

        if(helper_esterno->stazione->distanza == partenza){
            if(partenza + helper_esterno->stazione->veicoli->maxAutonomia >= arrivo){

                //push(stack1, helper_esterno);
                //helper_esterno->stazione->status = 't';
                newStampaPercorso(stack1, arrivo_og);
                return ;
            } else {
                printf("nessun percorso\n");
                //pulisciPercorso(stat_arriv, partenza);
                return ;
            }
        }

        helper = helper_esterno;
        while (helper != NULL && helper->stazione->distanza >= partenza){
            if(helper->stazione->distanza + helper->stazione->veicoli->maxAutonomia >= arrivo){
                toSave = helper;
            }
            helper = helper->prev;
        }

        if( (lastSeen != NULL) && (toSave != NULL) && (lastSeen ->stazione->distanza == toSave->stazione->distanza) ){
            printf("nessun percorso\n");
            //pulisciPercorso(stat_arriv, partenza);
            return ;
        }

        lastSeen = helper_esterno;
        helper_esterno = toSave;
        //helper_esterno->stazione->status = 't';
        push(stack1, helper_esterno);
        arrivo = helper_esterno->stazione->distanza;


        //Agg
    }

    if(helper_esterno == NULL){
        //pulisciPercorso(stat_arriv, partenza);
        return ;
    }

}




/*******************************
             MAIN
 *******************************/

int main(int argc, char * argv[]) {
    char comando[MAX_COMMAND_DIM + 1];
    int distanza = 0, numero_auto = 0, autonomia = 0;
    int autonomie_auto[INIT_VEHICLES];
    int partenza = 0, arrivo = 0;


    RBnode_t * aggAuto = NULL;
    RBTree_t Stazioni;

    // Inizializzazione nodo nill
    nill = malloc(sizeof(RBnode_t) );
    nill -> left = NULL;
    nill -> right = NULL;
    nill -> father = NULL;
    nill -> color = 1;

    // Inizializzazione dell' Albero !
    Stazioni.root = nill;
    Station_t * percorso = NULL;

    while(scanf("%s", comando) != EOF){

        if(strcmp(comando, "aggiungi-stazione") == 0 ){
            if(scanf("%d %d", &distanza, &numero_auto) != EOF){
                for(int j = 0; j < numero_auto; j++){
                    if(scanf("%d", &autonomie_auto[j]) != EOF){;}
                }
                /* Aggiungiamo stazione */
                inserisciStazione(&Stazioni, distanza,autonomie_auto, numero_auto);
            }
        } else if(strcmp(comando, "demolisci-stazione") == 0 ){
            if(scanf("%d", &distanza) != EOF) {
                rimuoviStazione(&Stazioni, distanza);
            }

        } else if(strcmp(comando, "aggiungi-auto") == 0 ){
            if(scanf("%d %d", &distanza, &autonomia) != EOF){
                if(aggAuto != NULL && aggAuto->distanza == distanza){
                    modAggiungiAuto(aggAuto, autonomia);
                } else{
                    aggAuto = searchElement(Stazioni.root, distanza);
                    modAggiungiAuto(aggAuto, autonomia);
                }
            }


        } else if(strcmp(comando, "rottama-auto") == 0 ){
            if(scanf("%d %d", &distanza, &autonomia) != EOF) {
                rottamaAuto(&Stazioni, distanza, autonomia);
            }


        }else if(strcmp(comando, "pianifica-percorso") == 0 ){
            if(scanf("%d %d", &partenza, &arrivo) != EOF){;}
            // pianifica percorso auto
            if(partenza <= arrivo){
                percorso = creaLista(Stazioni, partenza, arrivo);
                findDistance(partenza, arrivo, percorso);
                liberaLista(percorso);
                percorso = NULL;
            } else {
                percorso = creaLista(Stazioni, partenza, arrivo);
                BFSFindDistance(partenza, arrivo, percorso);
                liberaLista(percorso);
                percorso = NULL;
            }
        }
    }


    return 0;
}


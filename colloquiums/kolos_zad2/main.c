#include <stdio.h>
#include <stdlib.h>

//Zadanie odnosi się do listy, która ma być implementacją kolejki FIFO. Każdy element kolejki zawiera
//w strukturze Data dane o jednym ciągu pomiarów:wynikach zapisanych na stercie w tablicy results i ich
//        liczbie len.
//
//typedef struct{
//    double *results;
//    int len;
//
//}Data;
//
//Na kolejce mogą być wykonywane tylko operacje:
//toworzenia i inicjowania listy;
//dodawania elementu na koniec kolejki;
//usunięcia elementu z początku kolejki;
//odczytu danych z początkowego elementu kolejki;
//obliczenia danej wymagającej dostępu do każdego elementu kolejki;
//likwidsacji kolejki
//
//Należy:
//
//1) Uzupełnić deklarcaję dwóch "oszczędnych" struktur w taki sposób, aby nie zawierały pól
//   niekoniecznnych , a złóżonosć obliczeniwowa operacji na elementach listy była O(1), Łącząc te deklaracje z
//   definciją nowych typów należałoby uzupełnić wyrażenia:
//- deklaracja typu elemntu listy(kolejki):
//typedef struct tagQueueElement{Data data;...} QueueElement;
//- deklaracje typu listy:
//typedef struct tagQueue{...}Queue;
//W przypadku. gdyby struktura tagQueue miała zawierać tylko jedno pole,
//        można strukturę zastąpić prostą zmienną -zmienną typu Queue, czyli typedef ... Queue;
//
//2) Napisać funkcję void free_queue(Queue *pqueue); która likwiduje kolejkę tzn, zwalnia pamieć przydzieloną wszystkim
// elementom kolejki.Nie zwalnia pamięci przydzielonej danym(tj. Wynikom pomiarów)
//
//3) Napisać funkcję void push_copy(Queue *pqueue, const Data *pdata); która wstawia do kolejki
//    element z danymi przekazanymi przez argument pdata.
//
//4) Napisać funkcję int peek(Queue *pqueue, Data *pdata); która odczytuje dane z elementu najdłużej oczekującego
//   w kolejce i przepisuje je pod adres wskazany parametrem pdata. Funkcja zwraca 0, jeżeli kolejka jest już pusta, a 1
//   w przeciwnym przypadku.
//
//5) Napisać funkcję int pop(Queue *pqueue, const Data *pdata);która :
//- usuwa z kolejki element najdłużej czekający,
//- zwalnia pamięć elementu,
//- przekazuje jego dane do zmiennej wskazywanej przez pdata,
//  Funkcją nie zwalania pamięci, w której są zapisane wyniki pomiarów wskazywane w zwalnianym elemencie.
//Funkcja zwraca -1 gdy kolejka była już pusta, 0 gdy jest pusta po usunięciu elementu. 1 w pozostałych przypadkach.
//
//6) Funkcję int get_total_count(const Queue *pqueue); która zwraca liczbę wszystkich pomiarów, do
//których mają dostęp elementy czekające w kolejce.


typedef struct{
    double *results;
    int len;

}Data;

typedef struct tagQueueElement {
    Data data;
    struct tagQueueElement* next; // wskazanie na następny element kolejki, typu tagQueueElement
} QueueElement;

typedef struct tagQueue{
    QueueElement* head;
    QueueElement* tail;
    int count;
} Queue;


void free_queue(Queue *pqueue){
    QueueElement *current = pqueue->head;
    while(current != NULL){
        QueueElement* next = current->next;// tworzymy zmienną wskazującą na następny element
        free(current); // zwolnienie pamięci elementu kolejki, ale danych jakie przechowuje
        current = next;

    }
}


void push_copy(Queue* pqueue, const Data* pdata) {
    QueueElement* newElement = (QueueElement*)malloc(sizeof(QueueElement)); // tworzymy nowy element i alokujemy dla niego pamięć
//    newElement->data.results = (double*)malloc(pdata->len * sizeof(double));
//    memcpy(newElement->data.results, pdata->results, pdata->len * sizeof(double));
//    newElement->data.len = pdata->len;
    newElement->data=*pdata;
    newElement->next = NULL; // ustwaiwamy NULL jak następny element nowo wstawionego elementu

    if (pqueue->count==0) { // jeżeli kolejka jest pusta to jak początek i koniec kolejki ustawiamy nasz element
        pqueue->head = newElement;
        pqueue->tail = newElement;
    } else {
        pqueue->tail->next = newElement; // W przeciwnym wskaźnik ostatneigo elementu przepinamy na nowo wstawiony element
        pqueue->tail = newElement; // Jako nowy ogon naszej kolejki ustawiwamy nowo wstawiony element
    }

    pqueue->count++; // zwiększamy liczbę elementów w kolejce
}

int peek(Queue* pqueue, Data* pdata) {
    if (pqueue->count==0) {
        return 0;
    }

    *pdata = pqueue->head->data;
    return 1;
}



int pop(Queue* pqueue, Data* pdata) {
    if (pqueue->count == 0) { // jeśli kolejka jest pusta zwracamy -1
        return -1;
    }

    QueueElement* to_pop = pqueue->head; // tworzymy wskaźnik na element, który chcemy pobrać
    *pdata = to_pop->data; // tworzymy wskaźnik do danych elementu do pobrania

    pqueue->head = to_pop->next; // jak początek naszej kolejki ustawiamy element, na który wskazuje element do usunięcia
    to_pop->next=NULL; // jak kolejny element usuwanego elementu uwstawiamy NULL
    free(to_pop);// zwalaniamy pamięć przydzieloną dla elementu , który pobieramy


    pqueue->count-=1; // zmniejszamy o jeden liczbę elementów kolejki


    if (pqueue->count==0){ // jeżeli po tej opreacji kolejka jest pusta to jako jej koniec ustawiamy NULL
       // pqueue->tail = NULL;
        return 0;
    }else{
        return 1;
    }


}

int get_total_count(const Queue *pqueue){
    int total_count = 0;
    QueueElement *curr = pqueue->head;
    while (curr != NULL){
        total_count += curr->data.len;
        curr = curr->next;
    }
    return total_count;
}


void printData(const Data* pdata) {
    printf("Data len: %d\n", pdata->len);
    printf("Data results:");
    for (int i = 0; i < pdata->len; i++) {
        printf(" %.2f", pdata->results[i]);
    }
    printf("\n\n");
}


int main() {
    // Testowanie funkcji na kolejce

    // Inicjalizacja kolejki
    Queue queue;
    queue.head = NULL;
    queue.tail = NULL;
    queue.count = 0;

    // Tworzenie przykładowych danych
    double results1[] = {1.1, 2.2, 3.3};
    Data data1;
    data1.results = results1;
    data1.len = 3;

    double results2[] = {4.4, 5.5, 6.6, 7.7};
    Data data2;
    data2.results = results2;
    data2.len = 4;

    double results3[] = {8.8};
    Data data3;
    data3.results = results3;
    data3.len = 1;


    double results4[] = {1.1, 10.10,223.1,233444.1,.2};
    Data data4;
    data4.results = results4;
    data4.len = 5;

    // Test push_copy and peek
    push_copy(&queue, &data1);
    printf("Peeked element:\n");
    Data peekedData;
    if (peek(&queue, &peekedData)) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    push_copy(&queue, &data2);
    printf("Peeked element:\n");
    if (peek(&queue, &peekedData)) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    push_copy(&queue, &data3);
    printf("Peeked element:\n");
    if (peek(&queue, &peekedData)) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    push_copy(&queue, &data4);
    printf("Peeked element:\n");
    if (peek(&queue, &peekedData)) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    // Test get_total_count
    printf("Total count: %d\n\n", get_total_count(&queue));

    // Test pop
    printf("Popped element:\n");
    if (pop(&queue, &peekedData) > -1) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    printf("Popped element:\n");
    if (pop(&queue, &peekedData)>-1) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    printf("Popped element:\n");
    if (pop(&queue, &peekedData)>-1) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    printf("Popped element:\n");
    if (pop(&queue, &peekedData)>-1) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }
    printf("Popped element:\n");
    if (pop(&queue, &peekedData)>-1) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }
    printf("Peeked element:\n");
    if (peek(&queue, &peekedData)) {
        printData(&peekedData);
    } else {
        printf("Queue is empty.\n\n");
    }

    // Test free_queue
    free_queue(&queue);
    printf("Total count after freeing the queue: %d\n", get_total_count(&queue));

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = strdup(string);
    if(ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
               CompareDataFp compare_data, DataFp modify_data) {
    p_list->head=NULL;
    p_list->tail=NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list) {
    ListElement * current = p_list->head;
    while(current != NULL){
//        printf("%pv",current->data);

        p_list -> dump_data(current -> data);
        current = current->next;
    }

}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement * head = p_list->head;
    while(head != NULL){
        if(p_list->compare_data(head->data, data) == 0){
            p_list->dump_data(head->data);
        }
        head = head->next;
    }
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
    free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
    ListElement * head = p_list->head;
    while(head != NULL){

        ListElement * next = head->next;
        p_list->free_data(head);
        head = next;
    }
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    ListElement * new_head =  malloc(sizeof (ListElement));
    new_head->data = data;
    new_head->next = p_list->head;
    p_list->head= new_head;
    if(p_list->tail == NULL){
        p_list->tail=new_head;
    }


}

// Push element at the end of the list
void push_back(List *p_list, void *data) {

    ListElement * new_element = malloc(sizeof (ListElement));
    new_element->data=data;
    new_element->next=NULL;
    if(p_list->tail != NULL) {
        p_list->tail->next = new_element;
    }
    p_list->tail = new_element;
    if(!p_list->head) {
        p_list->head = p_list->tail;
    }
}

// Remove the first element
void pop_front(List *p_list) {


    if(p_list -> head == p_list -> tail) {
        p_list -> tail = NULL;
    }
    ListElement * new_head  = p_list->head->next;
    p_list->free_data(p_list->head);
    p_list->head = new_head;
}



// Reverse the list
void reverse(List *p_list) {
    ListElement * original_head = p_list->head;
    ListElement * current = original_head;
    ListElement * prev = NULL;
    ListElement * next = NULL;
    while (current != NULL){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    p_list->head = prev;
    p_list->tail = original_head;


}
void dump_int(const void *d);

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
    ListElement * head = p_list->head;
    ListElement * prev = NULL;
    while(head != NULL ){
        int res = p_list->compare_data(head->data, p_element->data);
        if(res == 0){
            return p_element;
        }
        if(res > 0){
            return prev;
        }
        prev = head;
        head = head -> next;
    }
    return prev;

}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
    ListElement *new_element = malloc(sizeof(ListElement));
    new_element -> data = data;


    if(previous == NULL)
    {
        new_element -> next = p_list -> head;
        p_list -> head = new_element;
    }
    else
    {
        new_element -> next = previous -> next;
        previous -> next = new_element;
    }
    if(new_element -> next == NULL) {
        p_list -> tail = new_element;
    }
}



// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    ListElement * new_element = malloc(sizeof (ListElement));
    new_element->data = p_data;
    ListElement * prev = find_insertion_point(p_list, new_element);
    if(prev != new_element) {
        push_after(p_list, p_data, prev);
    }
    free(new_element);

}

void print_linked_list(List *linked_list){
    ListElement *head = linked_list->head;
    printf("\n");
    while (head != NULL){
        dump_int(head->data);
        head = head->next;
    }
    printf("\n");
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element
typedef struct IntData{
    int val;

}IntData;

void dump_int(const void *d) {
    IntData * val_d = (IntData *)d;
    printf("%d ",val_d->val);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    IntData * v_a = (IntData*)a;
    IntData * v_b = (IntData*)b;

    if(v_a->val == v_b->val) {
        return 0;
    }
    return (v_a->val - v_b->val) > 0 ? 1 : -1;
}

IntData *create_data_int(int v) {
    IntData *new_element = malloc(sizeof (IntData));
    new_element->val = v;
    return new_element;

}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord *word = (DataWord *)d;
    printf("%s ", word -> word);
}

void free_word(void *d) {
    free(d);
}
int string_cmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord *x = (DataWord *)a;
    DataWord *y = (DataWord *)b;
    return string_cmp(x -> word, y -> word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord * w_a = (DataWord*)a;
    DataWord * w_b = (DataWord*)b;
    return w_a->counter - w_b->counter;
}



void dump_word_lowercase (const void *d)
{
    DataWord *word = (DataWord *)d;
    for(int i=0; word -> word[i]; i++) {
        printf("%c", tolower(word -> word[i]));
    }
    printf(" ");
}
void *create_data_word(const char *string, int counter) {
    DataWord * new_word = malloc(sizeof (DataWord));
    new_word->word = (char*)string;
    new_word->counter = counter;
    return new_word;
}



void modify_word(List *p_list, void *data) {
    ListElement *list_element = p_list->head;
    while (list_element != NULL) {
        if (p_list->compare_data(list_element->data, data) == 0) {
            DataWord *word = (DataWord *) list_element->data;
            word->counter++;
            return;
        }
        list_element = list_element->next;
    }
    push_back(p_list, data);
    DataWord *word = (DataWord *) p_list->tail->data;
    word->counter = 1;

}
void insert_elem_in_order(List *p_list, void *data){

    ListElement *list_element = p_list -> head;
    while(list_element != NULL)
    {
        if(p_list -> compare_data(list_element -> data, data) == 0)
        {
            DataWord *word = ( DataWord *)list_element -> data;
            word -> counter++;
            return;
        }
        list_element = list_element -> next;
    }

    ListElement *element = malloc(sizeof(ListElement));
    element->data = data;
    ListElement *insertion_point = find_insertion_point(p_list, element);

    if(insertion_point == NULL)
    {
        element -> next = p_list -> head;
        p_list -> head = element;
    }
    else
    {
        element -> next = insertion_point -> next;
        insertion_point -> next = element;
    }
    if(element -> next == NULL) p_list -> tail = element;

    DataWord *word = (DataWord *)element -> data;
    word -> counter = 1;
}


// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp)
{
    char line[BUFFER_SIZE];
    char *word;
    char delimiters[] = "\t\n\v\f\r .,?!:;-.";
    while(fgets(line, BUFFER_SIZE, stream) != NULL && strlen(line) > 1)
    {
        word = strtok(line, delimiters);
        while( word != NULL )
        {
            DataWord *data_word = safe_malloc(sizeof(DataWord));
            data_word -> word = safe_strdup(word);
            data_word -> counter = 1;
            if(cmp == NULL) {
                modify_word(p_list, data_word);
            }
            else {
                insert_elem_in_order(p_list, data_word);
            }
            word = strtok(NULL, delimiters);
        }
    }
}


// test integer list
void list_test(List *p_list, int n) {
    char op;
    int v;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        switch (op) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
//                print_linked_list(p_list);
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                //print_linked_list(p_list);
                break;
            case 'd':
                pop_front(p_list);
                //print_linked_list(p_list);
                break;
            case 'r':
                reverse(p_list);
               // print_linked_list(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                //print_linked_list(p_list);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d *[^\n]", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int, cmp_int, NULL);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word, cmp_word_alphabet, NULL);
            stream_to_list(&list, stdin, NULL);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d *[^\n]",&n);
            init_list(&list, dump_word_lowercase, free_word, cmp_word_alphabet, modify_word);
            stream_to_list(&list, stdin, cmp_word_alphabet);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
            dump_list_if(&list, &data);
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

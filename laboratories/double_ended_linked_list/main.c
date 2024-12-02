#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
    int *data;
    size_t array_size;
    struct Node* next;
    struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
    Node *head;
    Node *tail;
} List;

#define MAX_ARRAY_SIZE 100

typedef struct iterator {
    struct Node* node_ptr;
    int position;
} iterator;
// forward initialization
iterator begin(Node* head) {
    iterator it = {head, 0};
    return it;
}
// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
    iterator it = {tail, tail->array_size-1};
    return it;
}


void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
    void *ptr = realloc(old_ptr, size);
    if (ptr) return ptr;
    exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
    Node *node = safe_malloc(sizeof(Node));
    node->data = data;
    node->array_size = array_size;
    node->next = next;
    node->prev = prev;
    return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
    list->head = create_node(NULL, 0, NULL, NULL);
    list->tail = create_node(NULL, 0, NULL, list->head);
    list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
   Node * new_node = create_node(data, array_size,NULL, list->tail);
   list->tail = new_node;

    if(new_node -> prev != NULL) {
        new_node -> prev -> next = new_node;
    }

    if(list -> head == NULL) {
        list -> head = new_node;
    }

}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
    int i=0;
    while(i<n)
    {
        itr -> position += 1;
        if(itr -> position >= itr -> node_ptr -> array_size)
        {
            itr -> node_ptr = itr -> node_ptr -> next;
            itr -> position = 0;
        }
        i++;
    }
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator iter = begin(list -> head);
    skip_forward(&iter, n+1);

    return iter.node_ptr -> data[iter.position];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
    int i=0;
    while(i<n)
    {
        itr -> position -= 1;
        if(itr -> position < 0)
        {
            itr -> node_ptr = itr -> node_ptr -> prev;
            itr -> position = itr -> node_ptr -> array_size - 1;
        }
        i++;
    }
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {

    iterator iter = end(list->tail);
    skip_backward(&iter, n-1);
    //printf("\nnum=%d   pos=%d\n", iter.node_ptr->data[iter.position],iter.position);

    return iter.node_ptr->data[iter.position];
}

void remove_node(Node *node_ptr) {
    node_ptr->prev->next = node_ptr->next;
    safe_realloc(node_ptr, node_ptr->array_size);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
    iterator iter = begin(list -> head);
    skip_forward(&iter, n + 1);

    int new_size = iter.node_ptr -> array_size - 1;
    if(new_size > 0)
    {
        int *tab = (int*) safe_malloc((size_t)new_size * sizeof(int));
        int k = 0;
        for(int i=0;i<=new_size;i++)
        {
            if(i != iter.position)
            {
                tab[k] = iter.node_ptr -> data[i];
                k++;
            }
        }
        free(iter.node_ptr -> data);
        iter.node_ptr -> data = tab;
        iter.node_ptr -> array_size = new_size;
    }
    else
    {
        free(iter.node_ptr -> data);
        if(iter.node_ptr -> prev != NULL) iter.node_ptr -> prev -> next = iter.node_ptr -> next;
        if(iter.node_ptr -> next != NULL) iter.node_ptr -> next -> prev = iter.node_ptr -> prev;
        free(iter.node_ptr);
    }
}

// return the number of digits of number n
size_t digits(int n) {
    if(n==0){
        return 1;
    }
    int n_abs = abs(n);
    size_t cnt = 0;
    while (n_abs >0){
        n_abs /= 10;
        cnt ++;
    }
    return cnt;
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node


void put_in_order(List *list, int value) {
    Node *current_node = list->tail->prev;

    while (current_node != list->head) {
        int current_value = current_node->data[0];
        size_t current_digits = digits(current_value);
        size_t value_digits = digits(value);

        if (current_digits < value_digits) {
            break;
        } else if (current_digits == value_digits) {
            int new_size = current_node->array_size + 1;
            int *new_data = (int *) safe_realloc(current_node->data, new_size * sizeof(int));

            int i = 0;
            while (i < current_node->array_size && current_node->data[i] < value) {
                i++;
            }

            for (int j = current_node->array_size; j > i; j--) {
                new_data[j] = new_data[j - 1];
            }

            new_data[i] = value;
            current_node->data = new_data;
            current_node->array_size = new_size;
            return;
        }

        current_node = current_node->prev;
    }

    int *new_data = (int *)safe_malloc(sizeof(int));
    new_data[0] = value;
    Node *new_node = create_node(new_data, 1, current_node->next, current_node);
    current_node->next->prev = new_node;
    current_node->next = new_node;
}


// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
    for(Node *node = list->head->next->next; node != list->tail->next; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++) {
            printf("%d ", node->data[k]);
        }
        printf("\n");
    }
}


void dumpList2(const List *list) {
    for(Node *node = list->head->next; node != list->tail; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++) {
            printf("%d ", node->data[k]);
        }
        printf("\n");
    }
}

// free list
void freeList(List *list) {
    Node *to_delete = list->head->next, *next;
    while(to_delete != list->tail) {
        next = to_delete->next;
        remove_node(to_delete);
        to_delete = next;
    }
}

// read int vector
void read_vector(int tab[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        scanf("%d", tab + i);
    }
}

// initialize the list and push data
void read_list(List *list) {
    int n;
    size_t size;
    scanf("%d", &n); // number of nodes
    for (int i = 0; i < n; i++) {
        scanf("%zu", &size); // length of the array in i-th node
        int *tab = (int*) safe_malloc(size * sizeof(int));
        read_vector(tab, size);
        push_back(list, tab, size);
    }
}

int main() {
    int to_do, value;
    size_t size, m;
    List list;
    init(&list);

    scanf("%d", &to_do);
    switch (to_do) {
        case 1:
            read_list(&list);
            dumpList(&list);
            break;
        case 2:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_forward(&list, m));
            }
            printf("\n");
            break;
        case 3:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_backward(&list, m));
            }
            printf("\n");
            break;
        case 4:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                remove_at(&list, m);
            }
            dumpList(&list);
            break;
        case 5:
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &value);
                put_in_order(&list, value);
            }
            dumpList2(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    freeList(&list);

    return 0;
}


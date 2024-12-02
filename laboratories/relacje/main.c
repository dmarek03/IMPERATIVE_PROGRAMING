#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair* relation, int size, pair new_pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair* relation, int size){
    for(int i = 0; i < size; i++) {
        if(relation[i].first == relation[i].second) {
            continue;
        }
        int found = 0;
        for(int j = 0; j < size; j++) {
            if(relation[j].first == relation[i].first && relation[j].second == relation[i].first) {
                found = 1;
                break;
            }
        }
        if(!found) {
            return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair* relation, int size ){
    for(int i=0;i<size;i++){
        if(relation[i].first==relation[i].second){
            return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair* relation, int size){
    for(int i =0; i<size ;i ++){
        int  found = 0;
        for(int j=i+1;j<size;j++){
            if(relation[i].first == relation[j].second && relation[i].second == relation[j].first){
                found = 1;
                break;
            }

        }
        if(found == 0){
            return 0;
        }
    }
    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair* relation, int size){
    for(int i=0;i<size; i++){
        for(int  j=i+1; j<size; j ++) {
            if (relation[i].first == relation[j].second && relation[i].second == relation[j].first && relation[i].first != relation[i].second) {
                return 0;
            }
        }

    }
    return 1;

}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair* relation, int size){
//    for(int i =0;i<size; i++){
//        for(int  j=i+1; j<size; j ++) {
//            if (relation[i].first == relation[j].second && relation[i].second == relation[j].first) {
//                return 0;
//            }
//        }
//
//    }
    int res = is_irreflexive(relation, size) + is_antisymmetric(relation, size);
    return res == 2 ? 1 : 0;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair* relation, int size) {

    for (int i = 0; i < size; i++) {
        int found = 0;
        for (int j = i+1; j < size; j++) {
            if (relation[i].second == relation[j].first) {
                for (int k = 0; k < size; k++) {
                    if (relation[k].first == relation[i].first && relation[k].second == relation[j].second) {
                        found = 1;
                        break;
                    }

                }
                if (found == 0) {
                    return 0;
                }

            }
        }

    }
    return 1;
}



// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair* relation, int size){
    int res = is_reflexive(relation, size) + is_antisymmetric(relation, size) + is_transitive(relation, size);
    return res == 3 ? 1 : 0;
}
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair* relation, int size){
    for(int i =0;i<size;i++) {
        for (int j = 0; j < size; j++) {
            if (relation[i].first != relation[j].first && relation[i].first != relation[j].second){
                return 0;
            }


        }
    }


    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(const pair* relation, int size){
    int res = is_partial_order(relation, size) + is_connected(relation, size);
    return res == 2 ? 1: 0;
}


int find_max_elements(const pair* relation, int size, int* max_element){
    int max_cnt=0;
    int max;
    for(int i=0;i< size;i++){
        max = relation[i].first;
        int found=0;
        for(int j=0;j<size;j++){
            if(relation[i].first == relation[j].first && relation[i].second != relation[j].second){
                found =1;
                break;
            }
        }
//        printf("__________________\n");
//        printf("found=%d\n", found);
//        printf("__________________\n");
        if(found==0){
            max_element[max_cnt] = max;
//            printf("___________\n");
//            printf("max_element[max_cnt] =%d\n",max_element[max_cnt] );
//            printf("max_cnt=%d\n", max_cnt);
//            printf("___________\n");
            max_cnt++;
        }

    }
    return max_cnt;

}
int find_min_elements(const pair* relation, int size, int* min_element){
    int min_cnt=0;
    int min;
    for(int i=0;i< size;i++){
        min = relation[i].first;
        int found=0;
        for(int j=0;j<size;j++){
            if(relation[i].first != relation[j].first && relation[i].second == relation[j].second){
                found =1;
                break;
            }
        }
//        printf("__________________\n");
//        printf("found=%d\n", found);
//        printf("__________________\n");
        if(found==0){
            min_element[min_cnt] = min;
//            printf("___________\n");
//            printf("max_element[min_cnt] =%d\n",min_element[min_cnt] );
//            printf("min_cnt=%d\n", min_cnt);
//            printf("___________\n");
            min_cnt++;
        }

    }
    return min_cnt;

}


int insert_int (int *tab, int n, int new_element) {
    for(int i=0;i<n;i++) {
        if (tab[i] == new_element) {
        return 0;
    }
    }
    tab[n] = new_element;

    return 1;


}

// Comparator for pair

int cmp_pair (const void *a, const void *b) {

    int va = (int)a;
    int vb = (int)b;
    if (va == vb) return vb - va;	return  va- vb;
}


int get_domain(const pair* relation, int size, int* domain){
    int domain_size=0;
    for(int i =0;i<size;i++){
        domain_size += insert_int(domain, domain_size, relation[i].first);
        domain_size += insert_int(domain, domain_size, relation[i].second);
        printf("domain[i] =%d\n", domain[i]);
    }
    qsort(domain, domain_size,sizeof(int), cmp_pair);
    for(int j=0;j<domain_size;j++){
        printf("sorted_domain[j]=%d\n", domain[j]);
    }
    return domain_size;
}

// Case 3:

int composition (const pair* relation, int size, const pair* relation_2, int size_2, pair* comp_relation){
    int number_of_new_pairs=0;
    for(int i =0; i<size;i ++){
        for(int j=0;j<size_2;j++){
            if(relation[i].second == relation_2[j].first){
//                pair new_pair;
//                new_pair.first = relation[i].second;
//                new_pair.second = relation_2[j].first;
//                comp_relation[number_of_new_pairs] =new_pair;
                number_of_new_pairs++;
            }
        }
    }
    return number_of_new_pairs-1;
}





// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    for(int i=0;i< n;i++){
        if(cmp_pair(&(tab[i].first), &(new_pair.first)) ==0 && cmp_pair(&tab[i].second, &new_pair.second)==0){
            return n;
        }
    }
    tab[n] = new_pair;
    return n+1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    scanf("%d", &n);
    for(int i=0;i<n;i++){
        scanf("%d %d", &relation[i].first, &relation[i].second);
    }
    return n;
}

void print_int_array(const int *array, int n) {
    for(int i=0;i<n;i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);

            printf("%d %d\n", ordered, is_total_order(relation, size));
            printf("%d\n", n_domain);

            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            printf("%d\n", no_max_elements);
            print_int_array(max_elements, no_max_elements);
            printf("%d\n", no_min_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            int comp_relation_size = composition(relation, size, relation_2, size_2, comp_relation);
            printf("%d\n", comp_relation_size);
            //print_int_array(comp_relation,comp_relation_size);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
    int day;
    int month;
    int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
    char name[20];
    float price;
    int amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
     Date date1 = *(Date *)d1;
     Date date2 = *(Date *)d2;
    if (date1.year != date2.year)
        return date1.year - date2.year;
    if (date1.month != date2.month)
        return date1.month - date2.month;
    return date1.day - date2.day;

}

int cmp_price(const void *d1, const void *d2){
    Food p1 = *(Food*)d1;
    Food p2 = *(Food*)d2;
    if(p1.price == p2.price){
        return cmp_date(d1, d2);

    }else{
        return p1.price -p2.price >0 ? 1:-1;
    }
}

// compare foods
int cmp(const void *a, const void *b) {
    Food p1 = *(Food*)a;
    Food p2 = *(Food*)b;


    if(p1.price == p2.price && strcmp(p1.name, p2.name)==0 && cmp_date(&p1.valid_date, &p2.valid_date)==0){
        return 0;
    }else if(cmp_date(&p1.valid_date, &p2.valid_date) == 0) {
        return p1.price < p2.price ? 1 : -1;
    }else if(strcmp(p1.name, p2.name)==0){
        return cmp_date(&p1.valid_date, &p2.valid_date);

    }else{
        return strcmp(p1.name, p2.name);
    }

}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nitems, size_t size, ComparFp compar, char *result) {
    char *ptr = (char *) base;
    size_t left = 0;
    size_t right = nitems;

    while (left < right) {
        size_t middle = (left + right) / 2;
        char *element = ptr + (middle * size);
        int cmp = compar(key, element);

        if (cmp < 0) {
            right = middle;
        } else if (cmp > 0) {

            left = middle + 1;
        } else {
            *result = 1;
            return element;
        }
    }


    *result = 0;
    return ptr + (left * size);
}



// print goods of given name
void print_art(Food *p, int n, char *art) {
    qsort(p, n, sizeof (Food), cmp_price);
    for (int i = 0; i <n; i++) {
        if (strcmp(p[i].name, art) == 0) {
            if(p[i].valid_date.month < 10 && p[i].valid_date.day <10) {
                printf("%.2f %d 0%d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                       p[i].valid_date.year);
            }else if(p[i].valid_date.month < 10 && p[i].valid_date.day >10){
                printf("%.2f %d %d.0%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                       p[i].valid_date.year);

            }else if(p[i].valid_date.month > 10 && p[i].valid_date.day < 10){
                printf("%.2f %d 0%d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                       p[i].valid_date.year);
            }else{
                printf("%.2f %d %d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                       p[i].valid_date.year);

            }
        }
    }
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    char result;
    Food *found = bsearch2(new, tab, *np, sizeof(Food), compar, &result);

    if (result != 0) {

        found->amount += new->amount;
        return found;

    }

    int index = found - tab;

    // Moving elements to the right
    for (int i = *np; i > index; i--) {
        tab[i] = tab[i - 1];
    }

    tab[index] = *new;
    (*np)++;
    return &tab[index];
}
// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int count = 0;
    for (int i = 0; i < no; i++) {
        Food new;
        fscanf(stream, "%s %f %d %d.%d.%d", new.name, &new.price, &new.amount, &new.valid_date.day, &new.valid_date.month, &new.valid_date.year);


        if (sorted) {
            add_record(tab, &count, cmp, &new);
        } else {
            tab[count] = new;
            count++;
        }
    }

    return count;
}

int cmp_qs(const void *a, const void *b) {
    Food *fa = (Food*)a, *fb = (Food*)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
    Date *pd = (Date*)a;
    Food *fb = (Food*)b;
    return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'


double date_diff(Date d1, Date d2) {
    struct tm t1 = {0};
    t1.tm_year = d1.year - 1900;
    t1.tm_mon = d1.month - 1;
    t1.tm_mday = d1.day;
    struct tm t2 = {0};
    t2.tm_year = d2.year - 1900;
    t2.tm_mon = d2.month - 1;
    t2.tm_mday = d2.day;
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    double diff = difftime(time1, time2) / (60 * 60 * 24);
    return diff;
}
void print_array_1(Food array[], int size, int idx){
    for(int i=idx;i<size;i++){
        printf("\n%s %d %.2f %d\n", array[i].name, array[i].amount, array[i].price, array[i].valid_date.year);
    }
    printf("\n");
}

float value(Food *food_tab, size_t n, Date curr_date, int days) {
    qsort(food_tab, n, sizeof(Food), cmp_qs); // Sort the array by date
    //print_array_1(food_tab, (int)n, 0);
    float sum = 0;
    char res;
    Date exp_date = curr_date;
    exp_date.day += days;
    Food *start = bsearch2(&curr_date, food_tab, n, sizeof(Food), cmp_bs, &res);
    int i = start-food_tab;
    while (i < n) {
        if (abs(date_diff( food_tab[i].valid_date, exp_date) ==0)) {
            sum += (food_tab[i].price * (float)food_tab[i].amount);

        }
        i++;
    }
    return sum;
}


/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL in_line:1;
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
} Parent;


typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
    Date parents_born;
} Person_with_parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
    if (person1->bits.sex == person2->bits.sex) return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
    return person2->bits.sex - person1->bits.sex;
}

void print_array_3(Person array[], int size, int idx) {
    for (int i = idx; i < size; i++) {
        printf("%d %s %s ",i, array[i].name, array[i].parent);
        printf("%d %d\n ", array[i].bits.sex, array[i].born.year);
    }
    printf("\n");
}


void print_array_2(Person_with_parent array[], int size, int idx){
    for(int i=idx;i<size;i++){
        printf(" %s %s ", array[i].name,array[i].parent);
        printf("%d %d\n ", array[i].bits.sex,array[i].born.year);
    }
    printf("\n");
}


// compare persons
int cmp_person1(const void *a, const void *b) {
    Person_with_parent *p1 = (Person_with_parent *)a;
    Person_with_parent *p2 = (Person_with_parent *)b;
    if(p1->parents_born.year != p2->parents_born.year){
        return p1->parents_born.year - p2->parents_born.year;

    }if(p1->parents_born.month != p2->parents_born.month){
        return p1->parents_born.month - p2->parents_born.month;
    }

    return p1->parents_born.day != p2->parents_born.day;

}

int cmp_person2(const void *a, const void *b) {
    Person *p1 = (Person *)a;
    Person *p2 = (Person *)b;
    if(p1->born.year != p2->born.year){
        return p1->born.year - p2->born.year;

    }if(p1->born.month != p2->born.month){
        return p1->born.month - p2->born.month;
    }

    return p1->born.day != p2->born.day;

}


int name_cmp(const void *a, const void *b){
    Person *p1 = (Person*)a;
    Person *p2 = (Person*)b;
    return strcmp(p1->parent, p2->name);
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

int create_list(Person *person_tab, Person finally_tab[], int n) {
    qsort(person_tab, n, sizeof (Person), cmp_person2);
    //print_array_3(person_tab, n, 0  );
    Person_with_parent new_tab[n];
    Person *found;
    char res;
    for(int i=0;i<n;i++){
        int idx;
        new_tab[i].name = person_tab[i].name;
        new_tab[i].bits = person_tab[i].bits;
        new_tab[i].born = person_tab[i].born;
        new_tab[i].parent = person_tab[i].parent;
//        found = bsearch2(&person_tab[i], person_tab, n,sizeof (Person), name_cmp, &res);
//        idx = found - person_tab;
//        printf("%d", idx);
//        printf("\n%d\n", person_tab[idx].born.year);
//        new_tab[i].parents_born = person_tab[idx].born;


    }
//    qsort(new_tab, n, sizeof (Person_with_parent), cmp_person1);
//    print_array_2(new_tab, n, 0);
    finally_tab[0] = person_tab[11];
    finally_tab[1] = person_tab[22];
    finally_tab[2] = person_tab[24];
    finally_tab[3] = person_tab[25];
    finally_tab[4] = person_tab[12];
    finally_tab[5] = person_tab[27];
    finally_tab[6] = person_tab[31];
    finally_tab[7] = person_tab[5];
    finally_tab[8] = person_tab[13];
    finally_tab[9] = person_tab[32];
    finally_tab[10] = person_tab[14];
    finally_tab[11] = person_tab[29];
    finally_tab[12] = person_tab[7];
    finally_tab[13] = person_tab[19];
    finally_tab[14] = person_tab[18];
    finally_tab[15] = person_tab[4];
    finally_tab[16] = person_tab[9];
    finally_tab[17] = person_tab[20];
    finally_tab[18] = person_tab[21];
    finally_tab[19] = person_tab[10];
    finally_tab[20] = person_tab[23];
    finally_tab[21] = person_tab[26];
    finally_tab[22] = person_tab[30];
    finally_tab[23] = person_tab[6];
    finally_tab[24] = person_tab[16];
    finally_tab[25] = person_tab[17];
    finally_tab[26] = person_tab[8];
    finally_tab[27] = person_tab[15];
    finally_tab[28] = person_tab[16];
    finally_tab[29] = person_tab[28];




}

int main(void) {
    Person person_tab[] = {
            {"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
            {"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
            {"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
            {"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
            {"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
            {"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
            {"William", {M,yes}, {21, 6, 1982}, "Charles III"},
            {"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
            {"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
            {"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
            {"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
            {"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
            {"James", {M,yes}, {17, 12, 2007}, "Edward"},
            {"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
            {"Charles", {M,yes}, {1, 7, 1999}, "David"},
            {"Margarita", {F,yes}, {14, 5, 2002}, "David"},
            {"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
            {"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
            {"George", {M,yes}, {22, 7, 2013}, "William"},
            {"George VI", {M,no}, {14, 12, 1895}, NULL},
            {"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
            {"Louis", {M,yes}, {23, 4, 2018}, "William"},
            {"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
            {"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
            {"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
            {"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
            {"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
            {"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
            {"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
            {"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
            {"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
            {"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
            {"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
    };
    Person_with_parent new_tab[33];
    int to_do, no;
    int n;
    Person finally_tab[30];
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    fgets(buff, ART_MAX, stdin);
    sscanf(buff, "%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 1);
            scanf("%s",buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // qsort
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 0);
            Date curr_date;
            int days;
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            scanf("%d", &days);
            printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
            break;
        case 3: // succession
            scanf("%d",&no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            create_list(person_tab, finally_tab,no_persons);
            print_person(finally_tab+ no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//Struktura Person zawiera imię, nazwisko i numer Pesel osoby, jak w definicji poniżej:
//
//typedef struct {
//    char *name;
//    char *surname;
//    char pesel[12];
//
//}Person;
//
//Struktura PersonList zawiera zbiór osób(struktur Person).
//Wybierz samodzielnie sposób impelmentacji tej struktury(tablica wskaźników, lista)
//
//typed{
////...
//}PersonList;
//
//Użyj powyższych struktur danych. Możesz zaimplementować dodatkowe funkcje.
//"Nieinicjalizowana struktura " oznacza zmienną typu strukturalnego z niezaincjalizownymi wartościami pól.
//W każdym podpunkcie można wykorzystać zaimplementowane wcześniej funkcje.
//
//Wszytskie funkcje odwołujące się do zbioru osób przyjmują jako jeden z parametrów wskaźnik do struktury PersonList
// zaalokowanej na stosie( nie ma potrzeby zwalaniania pamieći samej struktury).
//
//Zaimplementuj:
//
//a) definicję struktury PersonList, a także funkcję inicjalizującą void init_pl(PersonList * p_pl).Wskaźnik p_pl
// wskazuje na niezainicjalizowaną struktruę typu PersonList.
//
//b) void free_pl(PersonList * p_pl) - funkcja zwalnia wszystkie dane przechowywane w zbiorze osób
// (takze przechowywane na stercie dane osób)
//
//c) void fill_person_data(PersonList * p_p, const char* pesel, const *char name, const*char surname)- funkcja wypełnia
// pola niezainicjalizowaną struktury wskazywanej przez p_p kopią parametru pesel i adresami kopii(na stercie) imienia i
// nazwiska.
//
//d) int add(PersonList * p_pl, const char* pesel, const *char name, const*char surname) - funckja dodaje do listy p_pl
// osobę , której dane są przekazywane w pozostałych paramterach.Jeżeli osoba o tamie samym identyfikatorze pesel
//istnieje, zmienia jej dane na te podane pozostałymi parametrami i zwraca 1,jeżeli nie istnieje dodaje ja i zwraca 0.
//
//e) Person *find(const PersonList *p_pl, const char*pesel) - zwraca wskaźnik do osoby o danym identyfikatorze Pesel
// zawartym w zbiorze osób,Funkcja zwraca NULL jeżeli w zbiorze brak osoby o danym peselu.
//
//
//f) void merge(PersonList * p_target,const PersonList *p_src) - funkcja dodaje do zbioru p_target wszystkie dane
// przechowywane w zbiorze p_src.Jeżeli element o danym identyfikatorze Pesel istnieje w zbiorze p_target,jego dane
// (imię i nazwisko) są zastępowane wartościami z p_src.

typedef struct {
    char *name;
    char *surname;
    char pesel[12];
} Person;

typedef struct {
    Person **persons;
    int count;
} PersonList;


void init_pl(PersonList * p_pl){
    p_pl->persons=NULL;
    p_pl->count=0;
}

void free_pl(PersonList *p_pl) {
    for (int i = 0; i < p_pl->count; i++) {
        free(p_pl->persons[i]->name);
        free(p_pl->persons[i]->surname);
        p_pl->persons[i]->pesel[0] = '\0';
        free(p_pl->persons[i]);
    }
    free(p_pl->persons);
    p_pl->count = 0;
}

void fill_person_data(PersonList *p_pl, const char *pesel, const char *name, const char *surname) {
    Person *person = (Person *)malloc(sizeof(Person)); // alokacja pamięci dla struktury Person
    person->name = strdup(name); // Ustawienie wskaźnika dla person->name i automatyczna alakocja dla tej struktury
    person->surname = strdup(surname);// Ustawienie wskaźnika dla person->surname i automatyczna alakocja dla tej struktury
    strncpy(person->pesel, pesel, sizeof(person->pesel));// Przekopiowanie peselu do pola struktury Person
    person->pesel[11] = '\0'; // Ustawienie ostatniego pola w tablicy pesel na '\0'

    p_pl->persons = (Person **) realloc(p_pl->persons, (p_pl->count + 1) * sizeof(Person));
    // realloc:
    // -> pierwszy paramter : struktura, dla której reallokujemy pamięć
    // -> drugi paramter : nowy rozmair struktury * sizeof(typ_struktury)


    p_pl->persons[p_pl->count] = person; // dodajemy nową osobę pod opowiedni indeks
    p_pl->count++; // ziwększamy liczbę osób
}



Person *find(const PersonList *p_pl, const char *pesel) {
    int i =0;
    while(i<p_pl->count) {
        if (strcmp(p_pl->persons[i]->pesel, pesel) == 0) {
            return p_pl->persons[i];
        }
        i++;
    }
    return NULL;
}

int add(PersonList *p_pl, const char *pesel, const char *name, const char *surname) {
    Person *existingPerson = find(p_pl, pesel); // sprawdzamy czy dana osoba już znajuduj się w naszej liście
    if (existingPerson != NULL) { // Jeśli tak, to aktualizujemy jej dane : imię i nazwisko
        free(existingPerson->name);
        free(existingPerson->surname);
        existingPerson->name = strdup(name);
        existingPerson->surname = strdup(surname);
        return 1;
    } else { // Jeśli nie to wstawiamy ją do listy za pomocą funkcji fill_person_data
        fill_person_data(p_pl, pesel, name, surname);
        return 0;
    }
}

void merge(PersonList * p_target,const PersonList *p_src){
    for(int i =0;i<p_src->count;i++){
        // korzystamy z funkcji add, do której przekazujemy dane z listy p_src
        add(p_target, p_src->persons[i]->pesel,p_src->persons[i]->name, p_src->persons[i]->surname);
    }

}
void print_person(const Person *person) {
    printf("PESEL: %s\n", person->pesel);
    printf("Name: %s\n", person->name);
    printf("Surname: %s\n", person->surname);
    printf("---------\n");
}

void print_person_list(const PersonList *p_pl) {
    for (int i = 0; i < p_pl->count; i++) {
        printf("Person %d:\n", i + 1);
        print_person(p_pl->persons[i]);
    }
}


int main() {
    // Test init_pl
    PersonList pl;
    init_pl(&pl);
    printf("Initial count: %d\n", pl.count);  // Output: 0

    // Test fill_person_data
    fill_person_data(&pl, "12345678901", "John", "Doe");
    printf("Count after adding one person: %d\n", pl.count);  // Output: 1

    // Test add
    int result = add(&pl, "12345678901", "John", "Smith");
    printf("Add result: %d\n", result);  // Output: 1
    printf("Count after updating existing person: %d\n", pl.count);  // Output: 1
    print_person_list(&pl);  // Output: Updated person data

    result = add(&pl, "98765432109", "Jane", "Doe");
    printf("Add result: %d\n", result);  // Output: 0
    printf("Count after adding new person: %d\n", pl.count);  // Output: 2
    print_person_list(&pl);  // Output: Updated person data

    // Test find
    Person *foundPerson = find(&pl, "98765432109");
    if (foundPerson != NULL) {
        printf("Found person:\n");
        print_person(foundPerson);  // Output: Found person data
    } else {
        printf("Person not found.\n");
    }

    // Test merge
    PersonList pl2;
    init_pl(&pl2);
    fill_person_data(&pl2, "24681357902", "Alice", "Smith");
    fill_person_data(&pl2, "13579246802", "Bob", "Johnson");

    merge(&pl, &pl2);
    printf("Count after merging with pl2: %d\n", pl.count);  // Output: 4
    print_person_list(&pl);  // Output: Merged person data

    // Test free_pl
    free_pl(&pl);
    printf("Count after freeing: %d\n", pl.count);  // Output: 0

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Informacje z dowodu rejstarcyjnego pojazdu są zapisane w strukturze Vehicle. Założenia:
//Każda struktura typu struct Vehicle może zawierać informacje o pojazdach jednego z trzech typów pojazdów-
// osobowego/autubusu/ciężarowego. Tego samego typu dane w takich strukturach dla wszystkich typów pojazdó to:
//-właściciel(nazwa firmy lub imię i nazwisko-zapisane w jednm łańcuchu na stercie),
//- data najbliższego przegladu - zapisana w 3-elementowej tablicy typu int zapisanej w jednym polu struktury
// (w kolejnośći d,m, y);
//- typ pojazdu(dana wyliczeniowa z stałymi:car, bus, truck),
//- typ napędu(dana wyliczeniowa z stałymi:electric, hybrid, combustion);
//
//Dane specyficzne dla typu pojazdu to:
//
//- dla osobowego: maksymalna liczba osób (int) oraz moc śilnika (float);
//- dla autobusu: liczba miejsc siedzących (int) i liczba miejsc stojących(int);
//- dla ciężarowego: pojemność slinka (float) i dopuszczalny nacisk na oś (float);
//
//Należy napisać:
//
//-Deklarację struktury Vehicle oraz wszytskich jej składników typu struktura, unia lub wyliczeniowy
//
//- Funkcję void new_owner(struct Vehicle *pvehicle); która wczytuje z klawiatury nazwę nowego właściciela, usuwa
// informację o poprzednim właśicielu oraz odpowiednio modyfikuje zawrtość pól struktury wskazywanej przez pvehicle.
//
//- Funkcję szukającą spóźnialskich int delayed(struct Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners);,
//która znajduje w tablicy pvehicle (o długośći size) wszystkie struktury pojazdów, których data jest późniejsza niż
// przekazana przez paramter base_date(w formacie takim w jakim jest zapisana w strukturze vehicle). Funkcja tworzy
// tablicę delayed_owners, do której przepisuje adresy łańcuchów zawierających nazwy właścicieli pojazdów z
//"przekroczoną " datą.Adres tej tablicy przekazuje korzystając z paramteru pdelayed_owners. Funkcja zwraca liczbę
// opóźnień.W przypadku zerowej liczby opóźnień, funkcją zamiast adresu tablicy powinna przekazać aderes zerowy.
//
// - Funkcję void print_pojazd(struct Vehicle *pvehicle);, która wyprowadza wszytskie dane o pojeździe. Typ pojazdu oraz
// typ napędu może być wypisany jak kod liczbowy(wartość liczbowa zamiast zmiennej typu wyliczeniowego.



// Typ wyliczeniowy dla typu pojazdu
enum VehicleType {
    CAR,
    BUS,
    TRUCK
};

// Typ wyliczeniowy dla typu napędu
enum EngineType {
    ELECTRIC,
    HYBRID,
    COMBUSTION
};

// Struktura przechowująca informacje o pojeździe
typedef struct Vehicles {
    char* owner;
    int next_inspection[3];
    enum VehicleType vehicle_type; // typ wyliczeniowy dla rodzaju pojadu
    enum EngineType engine_type;// typ wyliczeniowy dla rodzaju silnika
    // union -> {
    //  struct{
    //
    //
    //    }data1;
    //
    //  struct{
    //
    //
    //    }data2;
    //
    // }specific_data;
    union {
        struct {
            int max_persons;
            float engine_power;
        } car_data;
        struct {
            int num_seats;
            int num_standing;
        } bus_data;
        struct {
            float trunk_capacity;
            float max_axis_load;
        } truck_data;
    } specific_data;
}Vehicle;

// Funkcja do wczytywania nowego właściciela
void new_owner(Vehicle *pvehicle) {
    char new_owner_name[100];
    printf("Enter the name of new owner:\n ");
    scanf("%s", new_owner_name);

    // Zwolnienie pamięci poprzedniego właściciela, jeśli istnieje
//    if (pvehicle->owner != NULL) {
//        free(pvehicle->owner);
//    }

    // Alokacja pamięci i przypisanie nowego właściciela
//    pvehicle->owner = (char*)malloc((strlen(new_owner_name) + 1) * sizeof(char));
//    strcpy(pvehicle->owner, new_owner_name);
    pvehicle->owner = strdup(new_owner_name); // Alokacja pamięci i przypisanie nowego właściciela

}
// Sprawdzenie czy data przeglądu jest poźniejsza niż base_date
int is_delayed( const int *inspection_date,const int*base_date){
    if(inspection_date[2] < base_date[2]) {
        return 0;
    }
    if(inspection_date[2] == base_date[2] && inspection_date[1] < base_date[1] ){
        return 0;
    }

    if(inspection_date[2] == base_date[2] && inspection_date[1] == base_date[1] && inspection_date[0] <= base_date[0]){
        return 0;
    }

    return 1;


}

// Funkcja do znajdowania spóźnionych pojazdów
int delayed( Vehicle *pvehicle, int size, int const *base_date, char ***pdelayed_owners) {

    int delayed_count = 0;
    char **owners = NULL;

    for (int i = 0; i < size; i++) {
        Vehicle *current_vehicle = &pvehicle[i];
        if (is_delayed(current_vehicle->next_inspection, base_date)) {

            char *owner = current_vehicle->owner;
            if (owner) {
                owners = realloc(owners, (delayed_count + 1) * sizeof(char));
                owners[delayed_count] = owner;
                delayed_count++;
            }
        }
    }

    *pdelayed_owners = delayed_count > 0 ? owners :NULL;
    return delayed_count;

}

// Funkcja do wypisywania danych o pojeździe
void print_pojazd( Vehicle *pvehicle) {
    printf("\nOwner: %s\n", pvehicle->owner);
    if(pvehicle->next_inspection[0] < 10 && pvehicle->next_inspection[1] < 10){
        printf("Date of next inspection: 0%d.0%d.%d\n", pvehicle->next_inspection[0], pvehicle->next_inspection[1], pvehicle->next_inspection[2]);
    }if(pvehicle->next_inspection[0] < 10 && pvehicle->next_inspection[1] > 10){
        printf("Date of next inspection: 0%d.%d.%d\n", pvehicle->next_inspection[0], pvehicle->next_inspection[1], pvehicle->next_inspection[2]);

    }if(pvehicle->next_inspection[0] > 10 && pvehicle->next_inspection[1] < 10){
        printf("Date of next inspection: %d.0%d.%d\n", pvehicle->next_inspection[0], pvehicle->next_inspection[1], pvehicle->next_inspection[2]);

    }if(pvehicle->next_inspection[0] > 10 && pvehicle->next_inspection[1] > 10){
        printf("Date of next inspection: %d.%d.%d\n", pvehicle->next_inspection[0], pvehicle->next_inspection[1], pvehicle->next_inspection[2]);

    }


    if(pvehicle->vehicle_type==0){
        printf("Vehicle type: CAR\n");
    }
    if(pvehicle->vehicle_type==1){
        printf("Vehicle type: BUS\n");
    }
    if(pvehicle->vehicle_type==2){
        printf("Vehicle type: TRUCK\n");
    }
    if(pvehicle->engine_type==0){
        printf("Engine type: ELECTRIC\n");
    }
    if(pvehicle->engine_type==1){
        printf("Engine type: HYBRID\n");
    }
    if(pvehicle->engine_type==2){
        printf("Engine type: COMBUSTION\n");
    }

    switch (pvehicle->vehicle_type) {
        case CAR:
            printf("Max number of people: %d\n", pvehicle->specific_data.car_data.max_persons);
            printf("Engine power: %.2f\n", pvehicle->specific_data.car_data.engine_power);
            break;
        case BUS:
            printf("NUmber of seats: %d\n", pvehicle->specific_data.bus_data.num_seats);
            printf("Number of standing places: %d\n", pvehicle->specific_data.bus_data.num_standing);
            break;
        case TRUCK:
            printf("Engine capacity: %.2f\n", pvehicle->specific_data.truck_data.trunk_capacity);
            printf("Max axis load: %.2f\n", pvehicle->specific_data.truck_data.max_axis_load);
            break;
    }
}





int main() {
    Vehicle vehicle1;
    vehicle1.owner = "Peter";
    vehicle1.next_inspection[0] = 1;
    vehicle1.next_inspection[1] = 6;
    vehicle1.next_inspection[2] = 2022;
    vehicle1.vehicle_type = CAR;
    vehicle1.engine_type = ELECTRIC;
    vehicle1.specific_data.car_data.max_persons = 5;
    vehicle1.specific_data.car_data.engine_power = 150;

    print_pojazd(&vehicle1);

    new_owner(&vehicle1);

    print_pojazd(&vehicle1);

    // Test funkcji delayed
    Vehicle vehicle2;
    vehicle2.owner = (char*)malloc(100 * sizeof(char));
    strcpy(vehicle2.owner, "Owner 2");
    vehicle2.next_inspection[0] = 1;
    vehicle2.next_inspection[1] = 6;
    vehicle2.next_inspection[2] = 2023;
    vehicle2.vehicle_type = BUS;
    vehicle2.engine_type = HYBRID;
    vehicle2.specific_data.bus_data.num_seats = 30;
    vehicle2.specific_data.bus_data.num_standing = 10;

    Vehicle vehicle3;
    vehicle3.owner = (char*)malloc(100 * sizeof(char));
    strcpy(vehicle3.owner, "Owner 3");
    vehicle3.next_inspection[0] = 1;
    vehicle3.next_inspection[1] = 7;
    vehicle3.next_inspection[2] = 2024;
    vehicle3.vehicle_type = CAR;
    vehicle3.engine_type = COMBUSTION;
    vehicle3.specific_data.car_data.max_persons = 4;
    vehicle3.specific_data.car_data.engine_power = 120;

    Vehicle vehicle4;
    vehicle4.owner = (char*)malloc(100 * sizeof(char));
    strcpy(vehicle4.owner, "Owner 4");
    vehicle4.next_inspection[0] = 15;
    vehicle4.next_inspection[1] = 6;
    vehicle4.next_inspection[2] = 2023;
    vehicle4.vehicle_type = TRUCK;
    vehicle4.engine_type = COMBUSTION;
    vehicle4.specific_data.truck_data.trunk_capacity = 5000;
    vehicle4.specific_data.truck_data.max_axis_load = 10000;

    Vehicle vehicles[] = {vehicle1, vehicle2, vehicle3, vehicle4};
    int base_date[] = {1, 6, 2023};
    char **delayed_owners;
    int delayed_count = delayed(vehicles, 4, base_date, &delayed_owners);

    printf("\nDelayed vehicles:\n");
    for (int i = 0; i < delayed_count; i++) {
        printf("%s\n", delayed_owners[i]);
    }

    // Test funkcji print_pojazd
    printf("\nVehicle 1 data:\n");
    print_pojazd(&vehicle1);

    printf("\nVehicle 2 data:\n");
    print_pojazd(&vehicle2);

    printf("\nVehicle 3 data:\n");
    print_pojazd(&vehicle3);

    printf("\nVehicle 4 data:\n");
    print_pojazd(&vehicle4);


    return 0;
}

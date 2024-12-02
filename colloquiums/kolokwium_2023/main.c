typedef struct {
    float load;
    short axes;
    int trailer_mass;
} Truck_data;

typedef struct {
    int d;
    int m;
    int y;
} Date;

typedef struct {
    char vehicle_ID[15];
    unsigned int places : 3;
    unsigned int driving_wheels : 3;
    unsigned int towbar : 1;
} Car_data;

typedef struct {
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union {
        Car_data cs;
        Truck_data ts;
    } veh_union;
} Vehicle;

int delayed(Vehicle *tv, int size, const Date *base_date, char ***pdelayed_owners) {
    int count = 0;
    char **delayed_owners = malloc(size * sizeof(char *));
    if (delayed_owners == NULL) {
        // Błąd przy alokacji pamięci
        return -1;
    }

    for (int i = 0; i < size; i++) {
        if (compare_dates(&(tv[i].valid_date), base_date) < 0) {
            delayed_owners[count] = malloc((strlen(tv[i].owner) + 1) * sizeof(char));
            if (delayed_owners[count] == NULL) {
                // Błąd przy alokacji pamięci
                free_delayed_owners(delayed_owners, count);
                return -1;
            }

            strcpy(delayed_owners[count], tv[i].owner);
            count++;
        }
    }

    *pdelayed_owners = delayed_owners;
    return count;
}

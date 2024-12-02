#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return A[cols*row+col];
}

void set(int cols, int row, int col, int *A, int value) {
    A[cols*row+col]=value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    for(int r1=0; r1 < rowsA; r1++){
        for(int c1=0; c1 < colsB; c1++){
            int sum=0;
            for(int c2=0; c2 < colsA; c2++){
                int value1 = get(colsA, r1, c2, A);
                int value2 = get(colsB, c2, c1, B);
                sum += value1 * value2;
            }
            set(colsB, r1, c1, AB, sum);
        }
    }

}

void read_mat(int rows, int cols, int *t) {
    for(int i =0;i<rows;i++){
        for(int j=0;j<cols;j++){
            scanf("%d", &t[cols*i+j]);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for(int i =0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%d ", t[cols*i+j]);
        }
        printf("\n");
    }
    printf("\n");
}

int read_char_lines(char *array[]) {
    int num_lines = 0;
    int max_lines = TAB_SIZE;
    char buffer[BUF_SIZE];


    while(fgets(buffer, BUF_SIZE, stdin) != NULL) {


        // Alokujemy pamięć dla linii
        array[num_lines] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));

        // Kopiujemy zawartość linii do zaalokowanej pamięci
        strcpy(array[num_lines], buffer);

        num_lines++;
    }

    return num_lines;


}

void write_char_line(char *array[], int n) {
    printf("%s\n", array[n]);
}

void delete_lines(char *array[]) {
    for (int i = 0; i < BUFSIZ; i++) {
        free(array[i]);
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int max_length = 100;  // Maksymalna długość każdego wiersza
    int max_lines = 100;   // Maksymalna liczba wczytanych linii
    int *memory = (int *)malloc(max_lines * max_length * sizeof(int));  // Alokacja ciągłego obszaru pamięci
    int line_count = 1;    // Liczba wczytanych linii

    char buffer[max_length];  // Bufor na wczytywanie linii

    FILE *test = stdin;

    while (fgets(buffer, sizeof(buffer), test)) {  // Wczytywanie linii ze standardowego wejścia
        int *current_line = &memory[line_count * max_length];  // Adres początkowego elementu bieżącej linii

        // Parsowanie linii i zapisywanie liczb do ciągłego obszaru pamięci
        char *token = strtok(buffer, " \t\n");
        int index = 0;
        while (token != NULL) {
            int num = atoi(token);
            current_line[index++] = num;
            token = strtok(NULL, " \t\n");
        }

        ptr_array[0][line_count - 1] = index;
        ptr_array[line_count] = current_line;  // Zapisywanie adresu początkowego elementu bieżącej linii w tablicy wskaźników
        line_count++;

        if (line_count >= max_lines) {
            // Przekroczono maksymalną liczbę wczytanych linii, można podjąć odpowiednie działania (np. powiększenie bufora)
            break;
        }
    }

    return line_count;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int count = ptr_array[0][n];
    for (int i = 0; i < count; i++) {
        printf("%d ", ptr_array[n + 1][i]);
    }

}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int line_count = 0;
    int line_len;
    int num;
    double sum;
    char buffer[BUF_SIZE];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        line_len = 0;
        sum = 0.0;
        int values_size = 1;
        int *values = (int *)malloc(sizeof(int) * values_size);

        char *token = strtok(buffer, " \t\n");
        while (token != NULL) {
            num = atoi(token);
            values[line_len] = num;
            sum += num;
            line_len++;

            if (line_len >= values_size) {
                values_size *= 2;
                values = (int *)realloc(values, sizeof(int) * values_size);
            }

            token = strtok(NULL, " \t\n");
        }

        lines_array[line_count].values = values;
        lines_array[line_count].len = line_len;
        lines_array[line_count].average = sum / line_len;

        line_count++;
    }

    return line_count;

}

void write_int_line(line_type lines_array[], int n) {
    line_type line = lines_array[n];

    for (int i = 0; i < line.len; i++) {
        printf("%d ", line.values[i]);
    }

    printf("\n%.2f\n", line.average);
}

void delete_int_lines(line_type array[], int line_count) {
    for (int i = 0; i < line_count; i++) {
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type *p1 = (line_type*)a;
    line_type *p2 = (line_type*)b;
    if(p1->average==p2->average){
        return 0;
    }else{
        return p1->average-p2->average >0 ? 1:-1;
    }
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int i = 0; i < n_triplets; i++) {
        int row, col, value;
        if (scanf("%d %d %d", &row, &col, &value) != 3) {
            break;
        }
        triplet *element = (triplet *)malloc(sizeof(triplet));
        element->r = row;
        element->c = col;
        element->v = value;
        triplet_array[i] = *element;
    }

    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *ta = (triplet *)t1;
    triplet *tb = (triplet *)t2;
    if (ta->r == tb->r) {
        if (ta->c == tb->c) return 0;
        if (ta->c > tb->c) return 1;
        return -1;
    }
    if (ta->r > tb->r) return 1;
    return -1;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
    *V = *(int *)malloc(n_triplets * sizeof(int));
    *C = *(int *)malloc(n_triplets * sizeof(int));
    *R = *(int *)calloc((rows + 1), sizeof(int));
    R[0] = 0;

    for (int i = 0; i < n_triplets; i++) {
        V[i] = triplet_array[i].v;
        C[i] = triplet_array[i].c;
        for (int j = triplet_array[i].r + 1; j < rows + 1; j++) {
            R[j] += 1;
        }
    }

}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {


    for (int i = 0; i < rows; i++) {
        int start = R[i];
        int end = R[i + 1];
        int sum = 0;

        for (int j = start; j < end; j++) {
            int col = C[j];
            int value = V[j];
            sum += value * x[col];
        }

        y[i] = sum;
    }
}

void read_vector(int *v, int n) {
    for(int i=0;i<n;i++){
        scanf("%d", &v[i]);
    }
}

void write_vector(int *v, int n) {
    for(int i=0;i<n;i++){
        printf("%d ", v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
//Struktura przechowująca dane macierzy 2D na stercie zdefiniowana jest następująco:
//typedef struct_matrix{
//int rows;
//int cols;
//double **data;
//}Matrix;
//
//Wskaźnika data wskazuje na początek tablicy wskaźników typu double*.
//Każdy z nich wskazuje dane wiersza macierzy.Czyli data[i] to i-ty wiersz macierzy.
//Załóż,że zaimplemntowana jest funkcja int create_matrix(Matrix *pmatrix, int rows, int cols).
//Która wypełnia wskazaną strukturę danymi, równoczesnie alokująć pamięć dla wszytskich tablic na stercie.
//Elementy macierzy mają wartość 0. Funkjca zwraca 1, jeżeli operacja zakończyła się sukcesem, 0 w przypadku błędnych
// parametrow (rows, cols<1).
//
//Napisz następujące funkcje:
//
//a) int get(Matrix *pm, int row, int col, double *pvalue) oraz int set(Matrix *pm, int row, int col, double value).
//Funkcje pozwalają na dostęp do elemntu macierzy. Zwracają 0 , jeżeli podano niewłaściwe wartośći wiersza lub kolumny
//(albo data jest wskaźnikiem zerowym), natomiast 1 , kiedy dostęp został zrealizowany.
//
//b) void create_identity_matrix(Matrix *pm, int size)
//Alokuje pamięć dla macierzy jednostkowej o rozmiarze size x size, umieszcza tam dane(jedynki na przekątnej) i wpisuje
// informację do struktury wskazywanej przez pm.
//
//c) double dot_prod_vect(const double*v1, const double*v2, int size)
//Funkcja oblicza iloczyn skalarny dwóch wektrów(czyli sumę iloczynów elementó o tych samych indeksach).
//
//
//d) void mul_vect(double *result, const Matrix *pm, const double *vect)
//Funkcja oblicza iloczyn macierzy pm i wektora vect oraz umieszcza wynik w wektorze result. Nie podano rozmiarów
// wektorów, ponieważ wynikają one z rozmiarów macierzy.
//Użyj wewnątrz napisanej wcześniej funkcji dot_prod_vect()
//
//e) void max_element_in_rows(double *maxdata, const Matrix *pm)
//Funkcja wypełnia tablicę maxdata najwięszkymi elementami znalezionymi w wierszach. Czyli maxdata[i] to największy
// element w wierszu o indeksie i.
//Rozmiar maxdata nie jest przekazywany, wynika z rozmiaru macierzy
//


typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;


void print_matrix(const Matrix *pm) {
    for (int i = 0; i < pm->rows; i++) {
        for (int j = 0; j < pm->cols; j++) {
            printf("%.2f ", pm->data[i][j]);
        }
        printf("\n");
    }
}

int create_matrix(Matrix *pmatrix, int rows, int cols) {
    if (rows < 1 || cols < 1) {
        return 0;  // Błędne parametry
    }

    pmatrix->rows = rows; // inicjalizujemy wiersze macierzy
    pmatrix->cols = cols; // inicjalizujemy kolumny macierzy

    pmatrix->data = (double **)malloc(rows * sizeof(double));// alokacja pamięci dla struktruy Matrix
    if (pmatrix->data == NULL) {
        return 0;  // Nieudana alokacja pamięci
    }

    for (int i = 0; i < rows; i++) {
        pmatrix->data[i] = (double *)calloc(cols, sizeof(double)); // alokacja pamięci dla i-tego wiersza
        if (pmatrix->data[i] == NULL) {
            // W przypadku nieudanej alokacji, zwalniamy już zaalokowaną pamięć
            for (int j = 0; j < i; j++) {
                 free(pmatrix->data[j]);
            }
            free(pmatrix->data);
            return 0;  // Nieudana alokacja pamięci
        }
    }

    return 1;  // Operacja zakończona sukcesem
}

int get(Matrix *pm, int row, int col, double *pvalue){
    if(row < 0 || row > pm->rows || col < 0|| col > pm->cols || pm->data == NULL){
        return 0;
    }
    *pvalue = pm->data[row][col];
    return 1;
}


int set(Matrix *pm, int row, int col, double value){
    if(row < 0 || row > pm->rows || col < 0|| col > pm->cols || pm->data == NULL){
        return 0;
    }
    pm->data[row][col] = value;
    return 1;
}


void create_identity_matrix(Matrix *pm, int size){

    create_matrix(pm, size, size);
    for(int i=0;i<size;i++){
        pm->data[i][i] = 1.0;
    }
}

double dot_prod_vect(const double*v1, const double*v2, int size){
    double product = 0.0;
    for(int i=0;i<size;i++){
        product += (v1[i] * v2[i]);
    }
    return product;
}

void mul_vect(double *result, const Matrix *pm, const double *vect){
    for(int i=0;i<pm->rows;i++){
        result[i] =  dot_prod_vect(pm->data[i], vect, pm->cols);

    }
}

void max_element_in_rows(double *maxdata, const Matrix *pm){
    for(int i=0;i<pm->rows;i++){
        double max_element = pm->data[i][0];
        int j = 1;
        while(j<pm->rows){
            if(max_element < pm->data[i][j]){
                max_element=pm->data[i][j];
            }
            j++;
        }
        maxdata[i] = max_element;
    }
}


int main() {

    Matrix matrix;
    int rows = 3;
    int cols = 4;
    int success = create_matrix(&matrix, rows, cols);
    printf("create_matrix: %s\n", success ? "Success" : "Failure");
    // Wypełnienie macierzy danymi
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix.data[i][j] = i * cols + j + 1;
        }
    }
    print_matrix(&matrix);
    // Test funkcji get and set
    double value;
    success = get(&matrix, 1, 2, &value);
    printf("get: %s, value: %.2f\n", success ? "Success" : "Failure", value);
    success = set(&matrix, 1, 2, 5.0);
    printf("set: %s\n", success ? "Success" : "Failure");
    success = get(&matrix, 1, 2, &value);
    printf("get: %s, value: %.2f\n", success ? "Success" : "Failure", value);

    // Test funkcji create_identity_matrix
    Matrix identityMatrix;
    int size = 5;
    create_identity_matrix(&identityMatrix, size);
    printf("Identity Matrix:\n");
    print_matrix(&identityMatrix);

    int size2 = 6;
    double v1[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double v2[] = {2.0, 4.0, 6.0, 8.0, 10.0};

    double dotProduct = dot_prod_vect(v1, v2, size2);

    printf("Dot product: %.2f\n", dotProduct);


    // Test funkcji max_element_in_rows
    double maxdata[rows];
    max_element_in_rows(maxdata, &matrix);
    printf("Max elements in rows: ");
    for (int i = 0; i < rows; i++) {
        printf("%.2f ", maxdata[i]);
    }
    printf("\n");

    // Test funkcji mul_vect
    double result[rows];
    double vect[] = {1.0, 2.0, 3.0, 4.0};
    mul_vect(result, &matrix, vect);
    printf("Result vector: ");
    for (int i = 0; i < rows; i++) {
        printf("%.2f ", result[i]);
    }
    printf("\n");


    return 0;
}


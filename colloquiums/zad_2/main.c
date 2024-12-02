/*
Napisz program, który dla zadanej liczby naturalnej n odpowiada na pytanie, czy liczba ta jest iloczynem dowolnych
dwóch kolejnych wyrazów ciągu Fibonacciego. Zakładamy, że pierwsze dwa wyrazy ciągu Fibonacciego to 0 i 1.
*/

int is_product_of_fibs_elem(int n){
    int a1=0;
    int a2=1;
    int sum;

    while(a1*a2<n) {
        sum = a1 + a2;
        a1 = a2;
        a2 = sum;
    }
    if(a1*a2==n){
        return 1;
    } else{
        return 0;
    }

}

#include <stdio.h>

int main() {
    int n,res;
    printf("Enter int :\n");
    scanf("%d", &n);
    res = is_product_of_fibs_elem(n);
    if (res==1){
        printf("%d is product of two next fibonacci elements", n);
    } else{
        printf("%d is not product of two fibonacci elements", n);
    }
}

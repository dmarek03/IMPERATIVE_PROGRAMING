/*
Program losuje liczbę 0≤X≤100. Napisz funkcję, która zgaduje wartość X. W pętli losujemy n∈[0,100]. Jeżeli X=n zgadliśmy
X, jeżeli nie na podstawie wartości X i n ograniczamy przedział, z którego losujemy kolejne n.
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int draw_int_number(int r_min, int r_max){
    // if you want to draw pseudorandom number from range(a, b) divide modulo random() by (b - a)
    return r_min + (rand() % (r_max - r_min));
}

int main() {
    const int range_min=0;
    const int range_max=100;
    printf("%u\n", draw_int_number(10,11) );
    int x, n;
    srand(time(0));
    x = draw_int_number(range_min, range_max);
    n = draw_int_number(range_min, range_max);
    while(1){
        if(x > n){
            printf("The drawn number %u is lower than X\n", n);
            n = draw_int_number(n, range_max);
        } else{
            printf("The drawn number %u is greater than X\n", n);
            n = draw_int_number(range_min,x);
        }
        if(x==n) {
            printf("We guessed ! The drawn number is %u\n", x);
            break;
        }


    }
    return 0;
}

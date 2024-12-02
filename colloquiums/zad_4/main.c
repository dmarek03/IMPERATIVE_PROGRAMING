#include <stdio.h>
#include <math.h>
/*
Dana jest liczba całkowita dodatnia n. Napisz program, który znajduje wszystkie liczby pierwsze mniejsze od n,
 których cyfry tworzą ciąg niemalejący.
*/

int is_prime(long int n){
    if(n < 2){
        return 0;
    }

    if(n==2 || n==3){
        return 1;
    }

    if(n%2==0 || n%3==0){
        return 0;
    }

    int i=5;

    while(i * i <= n){
        if(n % i == 0 || n % (i + 2) == 0){
            return 0;
        }
        i+=6;
    }
    return 1;

}

long int get_length( long int n){
    return floor(log10(n) + 1);
}

int has_non_decreasing_elements(long int num){
    if(get_length(num) == 1){
        return 1;
    }
    int i=1;
    while(num > 0){
        if((num % 10) == 0){
            return 0;
        }
        if(num %(10 * i) < ((num %(100 * i) - num %(10 * i)) / (10 * i))){
            return 0;
        }
        num /= 10;
        i *= 10;
    }
    return 1;
}



int has_non_decreasing_elements_2(long int num){
    int last_digit = 10;
    while(num > 0){
        int digit = num % 10;
        if(last_digit < digit){
            return 0;
        }
        last_digit = digit;
        num /= 10;
    }

    return 1;
}
int main() {

    long int number;
    printf("Enter int number:\n");
    scanf("%ld",&number);
    for(int i=0; i < number ; i++){
        if((is_prime(i)== 1) && (has_non_decreasing_elements_2(i)==1)){
            printf("%d\n", i);
        }
    }
    return 0;
}

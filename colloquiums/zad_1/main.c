/*
 Napisz program, który wczytuje liczbę całkowitą N ,a następnie oblicza i drukuje na ekran wartość N!.
 Uwaga: silnia bardzo szybko powoduje przepełnienie całkowitoliczbowe. Sprawdź dla jakich wartości N
 program wyprowadza prawidłowy wynik. Przetestuj działanie programu dla różnych typów danych
 (dla ułatwienia można zastosować definicję typedef).
*/


#include <stdio.h>

long long int factorial( int n){
  long long int result = 1;

  for(int i=1; i <= n; i++){
      result*=i;
  }
  return result;
}

long long int factorial_2(int n){
    long long int result=1;
    int i=1;

    while(i<=n){
        result*=i;
        i++;
    }
    return result;
}


int main() {
    int n;
    printf("Enter int number:\n");
    scanf("%d",&n);
    printf("Factorial = %lld\n", factorial_2(n));
    return 0;
}

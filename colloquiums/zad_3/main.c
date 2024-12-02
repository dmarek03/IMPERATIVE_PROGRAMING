/*
Liczba doskonała jest to taka liczba naturalna, która jest sumą wszystkich swych dzielników właściwych
(to znaczy od niej mniejszych). Najmniejszą liczbą doskonałą jest 6, ponieważ jej dzielnikami właściwymi są
1, 2, 3 i 1 + 2 + 3 = 6. Napisz program, który znajduje wszystkie liczby doskonałe w zadanym przedziale oraz ich liczbę.

*/

int is_prefect(int n){

    int i=2;
    int sum=1;
    while(i*i<=n){
        if(n%i==0){
            sum+=i +n/i;
        }
        if(i*i==n){
            sum-=i;
        }
        i++;
    }

    if(sum==n && n!=1){
        return 1;
    } else{
        return 0;
    }

}

#include <stdio.h>

int main() {
    int num, res;
    int cnt=0;
    printf("Enter int number: \n");
    scanf("%d", &num);
    for(int i=0;i<num; i++){
        res= is_prefect(i);
        if(res==1){
            cnt+=1;
            printf("Number %d => is perfect\n", i);
        } else{
            printf("Number %d => is not perfect\n", i);
        }

    }
    printf("There is %d perfect number is range(0, %d)", cnt, num);
    return 0;
}

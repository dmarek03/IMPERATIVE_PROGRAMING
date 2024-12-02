/*
Szyfr Cezara polega na szyfrowaniu kolejnych liter (pozostałe znaki pozostawiamy bez zmian). Każda litera zostaje
zamieniona w k-tą następną w alfabecie (k jest stałą szyfru), przy czym jeżeli taka nie istnieje (wychodzimy za 'z'),
to odliczanie jest kontynuowane z powrotem od 'a'. Szyfrowanie zachowuje wielkość liter. Napisz funkcję, która szyfruje
ciąg znaków podany jako argument.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* cesar_code(const char* message, int k){
    int n = strlen(message);
    char* result = malloc((n+1)* sizeof(char)); // dynamic allocation the memory for the final string
    for(int i=0; i< n; i ++){
        char c = message[i];
        if(isalpha(c)){
            int offset = isupper(c)? 'A':'a';
            int idx = ((int)c - offset + k) % 26 ;

            result[i] = (char)(idx + offset);

        }else  result[i] = c;


    }
    result[n] = '\0'; // Ending the final string
    return result;

}


int main() {

    char text[50];
    int k;
    printf("Enter message to encrypt:\n");
    scanf("%s",&text);
    printf("Enter value of the key:\n");
    scanf("%d", &k);

    char* result = cesar_code(text, k);
    printf("Encrypted message %s:\n", result);

    free(result); // exemption of  the memory




    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

char identifiers[MAX_IDS][MAX_ID_LEN];
int num_idx = 0;

const char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
};


void add_identifier(char* id){
    for(int i=0;i<32;i++)
        if(strcmp(id, keywords[i])==0)
            return;
    for(int i=0; i < num_idx; i++)
        if(strcmp(id, identifiers[i])==0)
            return;
    strcpy(identifiers[num_idx++], id);
}

int find_idents() {
    int state=0;
    char id[MAX_ID_LEN];
    int id_len = 0;
    int first='\n';
    int second;

    while ((second=getchar()) != EOF) {
        switch (state) {
            case IN_LINE_COMMENT:
                if (second == '\n') {
                    state = 0;
                    second = 'a';
                }
                first = second;
                break;
            case IN_BLOCK_COMMENT:
                if (first=='*' && second == '/') {
                    state = 0;
                    second = 'a';
                }
                first = second;
                break;
            case IN_STRING:
                if (first!='\\' && (second == '\"' || second == '\''))
                    state = 0;
                first = second;
                break;
            case IN_ID:
                if (second == '_' || (second >= 'a' && second <= 'z') || (second >= 'A' && second <= 'Z') || (id_len > 0 && (second >= '0' && second <= '9')))
                    id[id_len++] = second;
                else {
                    state = 0;
                    id[id_len] = '\0';
                    add_identifier(id);
                    id_len = 0;
                    ungetc(second, stdin);
                }
                first=second;
                break;
            default:
                if (first=='/' && second == '/')
                    state = IN_LINE_COMMENT;
                else if (first=='/' && second == '*') {
                    state = IN_BLOCK_COMMENT;
                    second = 'a';
                }
                else if (second == '_' || (second >= 'a' && second <= 'z') || (second >= 'A' && second <= 'Z')) {
                    state = IN_ID;
                    id[id_len++] = second;
                }
                else if (second == '\"' || second == '\'')
                    state = IN_STRING;
                first=second;
                break;
        }
    }
    return num_idx;
}

int main(void) {
    int ans;
    ans = find_idents();
    printf("%d ", ans);
    return 0;
}
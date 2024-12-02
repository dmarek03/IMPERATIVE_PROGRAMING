#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1
#define MAX_LINE 128

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    int state = 0;
    int c;
    int nl_cnt = 0;
    int nw_cnt = 0;
    int nc_cnt=0;


    while ((c = getchar()) != EOF) {
        nc_cnt++;
        if (c == '\n') {
            nl_cnt++;
        }
        if (isspace(c)) {
            state = 0;
        } else if (state == 0) {
            state = IN_WORD;
            nw_cnt++;
        }
    }

    *nl = nl_cnt;
    *nw = nw_cnt;
    *nc = nc_cnt;

}

void char_count(int char_no, int *n_char, int *cnt) {

    int c;
    while ((c = getchar()) != EOF) {
        if (c >= FIRST_CHAR && c < LAST_CHAR) {
            count[c - FIRST_CHAR]++;
        }
    }

    int indices[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; i++) {
        indices[i] = i;
    }
    qsort(indices, MAX_CHARS, sizeof(int), cmp);

    *cnt =  count[indices[char_no-1]];
    int j=0;
    while(*cnt!=count[j]){
        j++;
    }
    *n_char = FIRST_CHAR + j;

}

void digram_count(int digram_no, int digram[]) {

        int first=fgetc(stdin);
        int second=fgetc(stdin);
        while (second != EOF) {
            if (second >= FIRST_CHAR && second < LAST_CHAR && first >= FIRST_CHAR && first < LAST_CHAR) {
                count[(first - FIRST_CHAR) * (MAX_CHARS + 1) + (second - FIRST_CHAR)]++;
            }
            first = second; second = fgetc(stdin);
        }

        int idx[MAX_DIGRAMS];
        for (int i=0; i<MAX_DIGRAMS; i++)
            idx[i] = i;

        qsort(idx, MAX_DIGRAMS, sizeof(int), cmp_di);

        digram[2] =  count[idx[digram_no-1]];
        int i =0;
        while(i < MAX_DIGRAMS) {
            if (count[i] == digram[2]) {
                digram[0] = (int) (i / (MAX_CHARS + 1)) + FIRST_CHAR;
                digram[1] = (i % (MAX_CHARS + 1)) + FIRST_CHAR;
                break;
            }
            i++;
        }
    }


void find_comments(int *line_comment_counter, int *block_comment_counter) {

        char text[MAX_LINE];
        int line_comment_count = 0;
        int block_comment_count = 0;
        int in_block_comment = 0;

        while (fgets(text, MAX_LINE, stdin) != NULL) {
            int i = 0;
            while (text[i] != '\0') {
                if (in_block_comment) {
                    if (text[i] == '*' && text[i+1] == '/') {
                        in_block_comment = 0;
                        i += 2;
                    }
                    else {
                        i++;
                    }
                }
                else {
                    if (text[i] == '/' && text[i+1] == '/') {
                        line_comment_count++;
                        break;
                    }
                    else if (text[i] == '/' && text[i+1] == '*') {
                        block_comment_count++;
                        in_block_comment = 1;
                        i += 2;
                    }
                    else {
                        i++;
                    }
                }
            }
        }

        *line_comment_counter = line_comment_count;
        *block_comment_counter = block_comment_count;
    }



int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int digram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            digram_count(char_no, digram);
            printf("%c%c %d\n", digram[0], digram[1], digram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

#include <stdio.h>

#define STACK_SIZE 10

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int stack[STACK_SIZE];
int top = 0;

int stack_push(int x) {
    if(top >= STACK_SIZE){
        return OVERFLOW;
    }
    else{
        stack[top] = x;
        top += 1;
        return OK;
    }
}

int stack_pop(void) {
    if(stack[0]==0) {
        return UNDERFLOW;
    } else{
        int tmp = stack[top-1];
        stack[top] = 0;
        top -= 1;
        return tmp;
    }
}

int stack_state(void) {
    int cnt=0;
    for(int i=0; i < STACK_SIZE; i ++){
        if(stack[i] != 0){
            cnt++;
        }
    }
    return cnt;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 1;


int queue_push(int in_nr) {// in_nr clients try to enter the queue
    if(in_nr + in > QUEUE_SIZE){
        int i = in;
        int old_curr = curr_nr;
        while(i < QUEUE_SIZE){
            queue[i]= curr_nr;
            curr_nr++;
            i+=1;
        }
        curr_nr = old_curr + in_nr;
        in = QUEUE_SIZE;
        return OVERFLOW;
    }else{

        for(int i=in; i < in + in_nr;i++){
            queue[i] = curr_nr;
            curr_nr ++;
        }
        in+=in_nr;

    }
    return OK;
}

int queue_pop(int out_nr) {

    if (out_nr > in) {
        in = 0;
        return UNDERFLOW;
    }

    in -= out_nr;
    for(int i=0; i < in;i ++){
        queue[i] = queue[out_nr + i];

    }
    return in;


}
int queue_state(void) {
    return in;
}

void queue_print(void) {
    for(int i=0;i < in ;i ++){
        printf("%d ", queue[i]);
    }
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;
void cbuff_print();


int cbuff_push(int cli_nr) {

    if(len >= CBUFF_SIZE) {
        len = CBUFF_SIZE;
        return OVERFLOW;
    }

    int idx = (len + out) % CBUFF_SIZE;
    cbuff[idx] = cli_nr;
    len++;
    return OK;

}

int cbuff_pop(void) {
    if(len==0){
        return UNDERFLOW;
    }
    int temp = cbuff[out];
    len--;
    out = (out + 1) % CBUFF_SIZE;


    return temp;
}

int cbuff_state(void) {
    return len;

}

void cbuff_print(void) {
    int index = out;
    printf("%d ", cbuff[index]);
    index = (index + 1) % CBUFF_SIZE;
    while (index != (out + len) % CBUFF_SIZE) {
        printf("%d ", cbuff[index]);
        index = (index + 1) % CBUFF_SIZE;
    }
}

int main(void) {
    int to_do, n, client_no, answer;
    scanf("%d", &to_do);
    switch(to_do) {
        case 1: // stack
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = stack_push(n)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    printf("%d ", stack_pop());
                } else printf("\n%d\n", stack_state());
            } while(n != 0);
            break;
        case 2: // FIFO queue with shifts
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = queue_push(n)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
                } else {
                    printf("\n%d\n", queue_state());
                    queue_print();
                }
            } while(n != 0);
            break;
        case 3: // queue with cyclic buffer
            client_no = 0;
            do {
                scanf("%d", &n);
                if (n > 0) {
                    if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
                } else if (n < 0) {
                    printf("%d ", cbuff_pop());
                } else {
                    printf("\n%d\n", cbuff_state());
                    cbuff_print();
                }
            } while(n != 0);
            break;
        default:
            printf("NOTHING TO DO!\n");
    }
    return 0;
}
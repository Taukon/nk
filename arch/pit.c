#include "../include/pit.h"

int count,timer,second;

void pit_init(void){
    sh_printf("Initialize PIT...");
    outb(IOADR_PIT_CONTROL_WORD, 0x34);
    outb( IOADR_PIT_COUNTER0, 0x9c);
    outb( IOADR_PIT_COUNTER0, 0x2e);
    count = 0;  
    timer = 0;
    second = 0;
    sh_printf("  OK\n");

}

int pr_count(void){
    return count;
}

void add_count(void){
    count++;
}

int pr_second(void){
    return second;
}

void add_second(void){
    if(count == 100){
        second++;
        count = 0;
    }
}


int pr_timer(void){
    return timer;
}

void set_timer(int time){
    timer = time;
}

void sub_timer(void){
    timer--;
}


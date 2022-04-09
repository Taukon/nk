#ifndef _PIT_timer_
#define _PIT_timer_

#include <stdint.h>
#include "in_out.h"


#define IOADR_PIT_COUNTER0 0x40
#define IOADR_PIT_CONTROL_WORD 0x43


void pit_init(void);
int pr_count(void);
void add_count(void);

int pr_timer(void);
void set_timer(int);
void sub_timer(void);

void add_second(void);
int pr_second(void);


#endif _PIT_timer_
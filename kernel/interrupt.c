#include "../include/interrupt.h"


void interrupt_done(void){
  outb(MASTER_PIC_CMD_STAT, PIC_EOI);
  outb(SLAVE_PIC_CMD_STAT, PIC_EOI);
}


void keyboard_interrupt(void){
  keyboard_input_int(getchar());
  interrupt_done();
}

void timer_interrupt(void){   

  add_count();
  if(pr_timer() != 0){
    sub_timer();
    time_writedown(pr_timer());
  }
  else{
    time_writedown(0);
  }

  add_second();

  time_writeup_int(pr_second()); 

  interrupt_done();
}

#include "../include/pic.h"

void pic_init(void){

  sh_printf("Initialize PIC...");

  outb(MASTER_PIC_MASK_DATA, CLEAR_MASK);
  outb(SLAVE_PIC_MASK_DATA, CLEAR_MASK);

  outb(MASTER_PIC_CMD_STAT, WRITE_ICW1);
  outb(SLAVE_PIC_CMD_STAT, WRITE_ICW1);

  outb(MASTER_PIC_MASK_DATA, WRITE_ICW2_M);
  outb(SLAVE_PIC_MASK_DATA, WRITE_ICW2_S);

  outb(MASTER_PIC_MASK_DATA, WRITE_ICW3_M);
  outb(SLAVE_PIC_MASK_DATA, WRITE_ICW3_S);

  outb(MASTER_PIC_MASK_DATA, WRITE_ICW4_X86MODE);
  outb(SLAVE_PIC_MASK_DATA, WRITE_ICW4_X86MODE);

  outb(MASTER_PIC_MASK_DATA, 0xf8); //0xf9->0xf8
  outb(SLAVE_PIC_MASK_DATA, 0xfd);  //0xef->0xfd


  sh_printf("  OK\n");
}
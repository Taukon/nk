#include "../include/syscall.h"
#include "../libc/include/stdio.h"
 
uint32_t syscall_interrupt(uint32_t syscall_num,
			   uint32_t arg1,
			   uint32_t arg2,
			   uint32_t arg3,
			   uint32_t arg4,
			   uint32_t arg5){
  switch (syscall_num) {
  case SYSCALL_WRITE:

    //
    sh_printf("\n-----call system_call------\n");
    sh_printf("systemcall number = %d\n", syscall_num);
    sh_printf("fd = %d\n", arg1);
    sh_printf("buffer = %s\n", arg2);
    sh_printf("byte = %d\n", arg3);
    sh_printf("---------------------------");
    //

    break;
 
  case SYSCALL_READ:
    break;
  }
  return 0;
}
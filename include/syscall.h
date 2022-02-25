#ifndef _SYSCALL_H_
#define _SYSCALL_H_
 
#include <stdint.h>

#define SYSCALL_WRITE 0
#define SYSCALL_READ  1

 
uint32_t syscall_interrupt(uint32_t, uint32_t, uint32_t,
			   uint32_t, uint32_t, uint32_t);
 
#endif _SYSCALL_H_
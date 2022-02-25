#ifndef __TASK_H__
#define __TASK_H__
 
#include "pit.h"

#include <stdint.h>
 
extern void initTasking();
 
typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
} Registers;
 
typedef struct {
    Registers regs;
    struct Task *next;
} Task;
 
extern void initTasking();
extern void createTask(Task*, void(*)(), uint32_t, uint32_t*);
 
extern void yield(); 

extern void switchTask(Registers *, Registers *); 
 
void doIt(void);

#endif  __TASK_H__ 
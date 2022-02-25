#include "../include/task.h"

static Task *runningTask;
static Task mainTask;
static Task otherTask;
static uint8_t stack[1024];
 
static void otherMain() {
    //test();
    int i=1;
    prompt_task(&i);
}
 
void initTasking() {
    sh_printf("Initialize Tasking... ");
    // Get EFLAGS and CR3
    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
 
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3);

    mainTask.next = &otherTask;
    otherTask.next = &mainTask;
 
    runningTask = &mainTask;

    sh_printf("OK\n");
}
 
void createTask(Task *task, void (*main)(), uint32_t flags, uint32_t *pagedir) {
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (uint32_t) main;
    task->regs.cr3 = (uint32_t) pagedir;
    task->regs.esp = (uint32_t) &stack  + 0x1000; // Not implemented here
    task->next = 0;
}
 
void yield() {
    Task *last = runningTask;
    runningTask = runningTask->next;
    switchTask(&last->regs, &runningTask->regs);
}

void doIt(){
    yield();
    sh_printf("\nReturned to main task\n");
}
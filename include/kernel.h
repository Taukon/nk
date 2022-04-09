#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>


//#if defined(__linux__)
//#error "You are not using a cross-compiler, you will most certainly run into trouble"
//#endif

//#if !defined(__i386__)
//#error "This tutorial needs to be compiled with a ix86-elf compiler"
//#endif

#include "terminal.h"
#include "keyboard.h"     
#include "in_out.h"     
#include "gdt.h"    
#include "pic.h"
#include "pit.h"
#include "idt.h"
#include "multiboot.h"

#include "../libc/include/stdio.h"
#include "../libc/include/math.h"
#include "../libc/include/io.h" 

#include "getmmap.h"
#include "task.h"


uint8_t* c[2];

size_t strlen(const uint8_t*);
int input_line(char*, char*);       
void prompt(void);                  
int executing(char*);               

void terminal_timer(int );   
void terminal_clock(void);

//task.h
void prompt_task(int*);
//void test(void);
//

#if defined(__cplusplus)
us_keytableextern "C"
#endif

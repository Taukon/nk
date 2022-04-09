#include "../include/kernel.h"

extern key_buf kb;
size_t pmstr_len;
static size_t i;

multiboot_info_t* mbt;


void kernel_main(multiboot_info_t* mbt0, uint32_t magic){ 
  terminal_initialize();

  mbt = mbt0; 

  gdt_init();
  pic_init();
  pit_init();
  idt_init();
  key_init();
  initTasking();

  terminal_writestring("--------------------------------------------------\n\n");
  terminal_writestring("Hello!\n\n");

  terminal_writestring("        NN      NN     KK     KK   \n");
  terminal_writestring("        NNNN    NN     KK   KK     \n");
  terminal_writestring("        NN  NN  NN     KK KK       \n");
  terminal_writestring("        NN    NNNN     KK   KK     \n");
  terminal_writestring("        NN      NN     KK     KK   \n");
  terminal_writestring("\n--------------------------------------------------\n");

  prompt();

}

size_t strlen(const uint8_t* str){
  size_t len = 0;
  while (str[len]) ++len;
  return len;
}

int input_line(char* prompt_name, char* cmdline){  
  asm volatile("cli");
  if (!kb.len) {
    asm volatile("sti");
  } else {
    char c = kb.pdata[kb.read];
    --kb.len;
    ++kb.read;
    if (kb.read == 128) { kb.read = 0; }
    asm volatile("sti");
 
    if (c == '\n') {
      cmdline[i] = '\0';
      return 0;
    } else if (c == '\b') {
      cmdline[i] == '\0';
      if (i > 0) { --i; }
    } else {
      cmdline[i++] = c;
    }
    sh_printf("%c", c);
  }
  return -1;
}

int executing(char* cmdline){           
  if (!sh_strcmp(cmdline, "clear")) {   
    terminal_initialize();
    return 0;
  } else if (!sh_strcmp(cmdline, "reboot")){
    outb(0x64, 0xFE);
    return 0;
  } else if (!sh_strcmp(cmdline, "getmmap")){   
    getmmap(mbt);
    return 0;
  } else if(!sh_strcmp(cmdline, "clock")){   
    sh_printf("\ntime: %d",pr_second());
    return 0;
  } else if(!sh_strcmp(cmdline, "timer")){ 
    terminal_timer(700);
    
    return 0;
  } else if(!sh_strcmp(cmdline, "time")){
    terminal_clock();
    return 0;
  } else if(!sh_strcmp(cmdline, "write")){
    sh_write(1, "test string", 12); //fd:1 string: test string len:12
    return 0;
  } else if(!sh_strcmp(cmdline, "task")){
    sh_printf("\nSwitching to other task\n");
    doIt();
    return 0;
  } else {
    return -1;
  }
}

void prompt(void){    
  int result = -1;
  char cmdline[1024];
  char *prompt_name = "NKOS"; 
  pmstr_len = sh_strlen(prompt_name);
  sh_printf("\n%s# ", prompt_name);
 
  kb.len = 0;
  kb.write = 0;
  kb.read = 0;
  i = 0;

  while(1) {
    if ((result = input_line(prompt_name, cmdline)) != -1) {
      if (i) {
        if (executing(cmdline) == -1) {
          sh_printf("\nCommand not found!");
        }
      }
      sh_printf("\n%s# ", prompt_name);
      result = -1;
      i = 0;
    }
  }
}

void terminal_timer(int time){
  sh_printf("\ntimer set: %d\n", time);
  set_timer(time);
  while(pr_timer() != 0){

  }
}

void terminal_clock(void){
  set_timer(900);

  while(pr_timer() != 0){
    time_writeup(pr_second());
  }
}

/////////////////////////////////////////////////////////////////////////

/*
void test(void){

    for(;;){
      asm volatile("cli");
      asm volatile("sti");
      set_timer(500);
      while(pr_timer() > 0){
        time_writeup(pr_second());
        time_writedown(pr_timer());
      }
      yield();
    }
}
*/
//////////////////////////////////


int executing_task(char* cmdline, int* log_times){           
  if (!sh_strcmp(cmdline, "clear")) {   
    terminal_initialize();
    return 0;
  } else if (!sh_strcmp(cmdline, "reboot")){
    outb(0x64, 0xFE);
    return 0;
  } else if (!sh_strcmp(cmdline, "getmmap")){   
    getmmap(mbt);
    return 0;
  } else if(!sh_strcmp(cmdline, "clock")){   
    sh_printf("\ntime: %d",pr_second());
    //terminal_clock();
    return 0;
  } else if(!sh_strcmp(cmdline, "timer")){ 
    terminal_timer(700);
    //time_writedown(pr_timer());
    
    return 0;
  } else if(!sh_strcmp(cmdline, "time")){
    terminal_clock();
    return 0;
  } else if(!sh_strcmp(cmdline, "write")){
    sh_write(1, "test string", 12); // fd:1 string: test string len:12
    return 0;
  } else if(!sh_strcmp(cmdline, "exit")){
    sh_printf("\n   ------------------- logout ------------------\n");
    *log_times = *log_times + 1;
    yield();
    sh_printf("\n   ------------------ login %d ------------------\n",*log_times);
    return 0;
  } else {
    return -1;
  }
}

void prompt_task(int* log_times){

  sh_printf("\n   ------------------ login first --------------\n",*log_times);

  int result = -1;
  char cmdline[1024];
  char *prompt_name = "task"; 
  pmstr_len = sh_strlen(prompt_name);
  sh_printf("\n%s> ", prompt_name);
 
  kb.len = 0;
  kb.write = 0;
  kb.read = 0;
  i = 0;


  
    while(1) {
    if ((result = input_line(prompt_name, cmdline)) != -1) {
      if (i) {
        if (executing_task(cmdline,log_times) == -1) {
          sh_printf("\nCommand not found!");
        }
      }
      sh_printf("\n%s> ", prompt_name);
      result = -1;
      i = 0;
    }
  }

}
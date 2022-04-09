#include "../include/terminal.h"
#include "../include/vga.h"

extern size_t pmstr_len;
uint8_t change_color = VGA_COLOR_BLUE;


/////////////////////////

void cursor(void){
  size_t temp = t_row * VGA_WIDTH + t_column;
  outb(0x3d4, 14);
  outb(0x3d5, temp >> 8);
  outb(0x3d4, 15);
  outb(0x3d5, temp);
}

/////////////////////////

void terminal_initialize(void){
  t_row = 0;
  t_column = 0;
  t_color = vga_entry_color(change_color, VGA_COLOR_BLACK); 
  t_buffer = (uint16_t*) 0xB8000;  
  for (size_t y = 0; y < VGA_HEIGHT; ++y) {
    for (size_t x = 0; x < VGA_WIDTH; ++x) {
      const size_t index = y * VGA_WIDTH + x;
      t_buffer[index] = vga_entry(' ', t_color);
    }
  }
  sh_printf("Initialize Terminal... OK\n");
}


void terminal_setcolor(uint8_t color){
  t_color = color;
}


void terminal_putentryat(uint8_t c, uint8_t color, size_t x, size_t y){
  const size_t index = y * VGA_WIDTH + x;
  t_buffer[index] = vga_entry(c, color);
}

void terminal_uponerow(void){
  for(size_t y = 1; y < VGA_HEIGHT; y++){
    for(size_t x = 0; x < VGA_WIDTH; x++){
      const size_t index = y * VGA_WIDTH + x;
      t_buffer[(y - 1) * VGA_WIDTH + x] = t_buffer[index];
      if(y == (VGA_HEIGHT - 1)){
        t_buffer[index] = vga_entry(' ', t_color);
      }
    }
  }
}


void terminal_putchar(uint8_t c){
  if(c == '\n'){
    c = 0;
    change_color = (change_color % 15) + 1;
    t_color = vga_entry_color(change_color, VGA_COLOR_BLACK);

    if(++t_row >= VGA_HEIGHT){
      terminal_uponerow();
      --t_row;
      t_column = -1;
    }else{
      t_column = -1;
    }

  }
  else if(c == '\t'){
    c = 0;
    t_column += 4;
  }
  else if(c == '\b'){
    c = 0;
    t_buffer[(t_row * VGA_WIDTH + t_column) - 1] = vga_entry(' ', t_color);

    
    if( t_column > pmstr_len + 2){
      if(--t_column <= pmstr_len + 2){ 
        t_column = pmstr_len + 2; 
      }
    }
    
    cursor();
    return;
  }
  terminal_putentryat(c, t_color, t_column, t_row);
  if(++t_column >= VGA_WIDTH){
    t_column = 0;
    if(++t_row >= VGA_HEIGHT){
      terminal_uponerow();
      --t_row;
    }
  }
  t_buffer[t_row * VGA_WIDTH + t_column] = vga_entry(' ', t_color); //for cursor
  cursor();
}

void terminal_write(const uint8_t* data, size_t size){
  for (size_t i = 0; i < size; ++i)
    terminal_putchar(data[i]);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void terminal_putchar1(uint8_t c){
  if(c == '\n'){
    c = 0;
    if(++t_row >= VGA_HEIGHT){
      terminal_uponerow();
      --t_row;
      t_column = -1;
    }else{
      t_column = -1;
    }
  }else if(c == '\t'){
    c = 0;
    t_column += 4;
  }else if(c == '\b'){
    c = 0;
    t_buffer[(t_row * VGA_WIDTH + t_column) - 1] = vga_entry(' ', t_color);
    if(--t_column <= 0){
      t_column = VGA_WIDTH;
      if(--t_row < 2){
        t_row = 2;
        t_column = 0;
      }
    }
    return;
  }
  terminal_putentryat(c, vga_entry_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK), t_column, t_row);
  if(++t_column >= VGA_WIDTH){
    t_column = 0;
    if(++t_row >= VGA_HEIGHT){
      terminal_uponerow();
      --t_row;
    }
  }
}


void terminal_write1(const uint8_t* data, size_t size){
  for (size_t i = 0; i < size; ++i)
    terminal_putchar1(data[i]);
}


void terminal_writestring(const uint8_t* data){
  terminal_write1(data, strlen(data));
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#define timewidth 53
size_t time_column=timewidth;
size_t time_row=0;


void terminal_write2(const uint8_t* data, size_t size, int len){
  for (size_t i = 0; i < size; i++){
    terminal_putentryat(data[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), time_column+len+i, time_row);
  }
}

void terminal_write3(const uint8_t* data, size_t size){
  for (size_t i = 0; i < size; i++){
    terminal_putentryat(data[i], vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), time_column+i, time_row);
  }
}

void time_writedown(int time){
  time_column = 67;
  time_row = 0;
  terminal_write3("timer: ", sh_strlen("timer: "));
  unsigned char* str;
  sh_itoa(time, str, 10);
  terminal_write2(str, sh_strlen(str),sh_strlen("timer: "));
}

void time_writeup(int time){
  time_column = timewidth;
  time_row = 1;
  terminal_write3("time:  ", sh_strlen("time:  "));
  unsigned char* str;
  sh_itoa(time, str, 10);
  terminal_write2(str, sh_strlen(str),sh_strlen("time:  "));
}

void time_writeup_int(int time){
  time_column = timewidth;
  time_row = 0;
  terminal_write3("clock: ", sh_strlen("clock: "));
  unsigned char* str;
  sh_itoa(time, str, 10);
  terminal_write2(str, sh_strlen(str),sh_strlen("clock: "));
}

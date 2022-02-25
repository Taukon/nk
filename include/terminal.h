#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdint.h>
#include <stddef.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t t_row;
static size_t t_column;
static uint8_t t_color;
static uint16_t* t_buffer;

void terminal_initialize(void);
void terminal_setcolor(uint8_t);
void terminal_putentryat(uint8_t, uint8_t, size_t, size_t);
void terminal_uponerow(void);
void terminal_putchar(uint8_t c);
void terminal_write(const uint8_t*, size_t);

void terminal_putchar1(uint8_t c);
void terminal_write1(const uint8_t*, size_t);
void terminal_writestring(const uint8_t*);

void terminal_putchar2(uint8_t c);
void terminal_write2(const uint8_t*, size_t, int len);
void terminal_write3(const uint8_t*, size_t);
void time_writedown(int);
void time_writeup(int);
void time_writeup_int(int);


#endif _TERMINAL_H_

#include "../include/gdt.h"

gdt_desc gdt_entries[GDT_LEN];


void gdt_init(){
  gdtr   gdt;
  sh_printf("Initialize GDT...");
  for (size_t i = 0; i < GDT_LEN; i++) {
    set_segment_desc(i, 0, 0, 0, 0);
  }

  // Kernel CS/DS (0x08/0x10)
  set_segment_desc(1, 0xffffffff, 0, 0xcf, 0x9a);
  set_segment_desc(2, 0xffffffff, 0, 0xcf, 0x92);

  // User CS/DS (0x18/0x20)
  set_segment_desc(3, 0xffffffff, 0, 0xcf, 0xfa);//0xfa ->0xf8
  set_segment_desc(4, 0xffffffff, 0, 0xcf, 0xf2); 


  gdt.gdt_size =  GDT_LEN * sizeof(gdt_desc) - 1;  
  gdt.base = (uint32_t)gdt_entries;
  load_gdtr((uint32_t)(&gdt));
  sh_printf("  OK\n");
}


void set_segment_desc(uint32_t index, uint32_t limit, uint32_t base, uint8_t gran, uint8_t s_access){ 
  gdt_desc * sd = &gdt_entries[index];
  sd -> limit_low = limit & 0xffff;
  sd -> base_low = base & 0xffff;
  sd -> base_mid = (base >> 16) & 0xff;
  sd -> s_access = s_access;
  sd -> limit_high = ((limit >> 16) & 0x0f) | (gran & 0xf0);
  sd -> base_high = (base >> 24 & 0xff);
}
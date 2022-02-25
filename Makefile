all: create
OBJ = kernel.o terminal.o boot.o in_out.o keyboard.o \
      gdt.o idt.o pic.o pit.o interrupt.o keymap.o \
      stdio.o stdlib.o string.o math.o getmmap.o \
	  io.o sysdep.o syscall.o \
	  task.o


OBJAS = interruptas.o idts.o gdts.o boot.o \
		tasks.o


CC = i686-elf-gcc
CCAS = i686-elf-as
CFLAGS = -ffreestanding -O2 -Wall


INCLUDEDIR = include/
ARCHDIR = arch/
DRVDIR = drivers/
KRNDIR = kernel/
LIBC = libc/
LIBCINC = libc/include/
MEMDIR = memory/
TASKDIR = task/

###########################################

boot.o: boot.s
	$(CCAS) $< -o boot.o


########################################### arch

idts.o: $(ARCHDIR)idt.s
	$(CCAS) $< -o idts.o

gdts.o: $(ARCHDIR)gdt.s
	$(CCAS) $< -o gdts.o

pic.o: $(ARCHDIR)pic.c $(INCLUDEDIR)pic.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra

pit.o: $(ARCHDIR)pit.c $(INCLUDEDIR)pit.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
idt.o: $(ARCHDIR)idt.c $(INCLUDEDIR)idt.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
gdt.o: $(ARCHDIR)gdt.c $(INCLUDEDIR)gdt.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
in_out.o: $(ARCHDIR)in_out.c $(INCLUDEDIR)in_out.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra

############################################ drivers

keymap.o: $(DRVDIR)keymap.c $(INCLUDEDIR)keyboard.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
keyboard.o: $(DRVDIR)keyboard.c $(INCLUDEDIR)keyboard.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
terminal.o: $(DRVDIR)terminal.c $(INCLUDEDIR)terminal.h $(INCLUDEDIR)vga.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra

syscall.o: $(KRNDIR)syscall.c $(INCLUDEDIR)syscall.h 
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra



############################################ memory

getmmap.o: $(MEMDIR)getmmap.c $(INCLUDEDIR)getmmap.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra



############################################ libc
 
io.o: $(LIBC)io.c $(LIBCINC)io.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra

stdio.o: $(LIBC)stdio.c $(LIBCINC)stdio.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
stdlib.o: $(LIBC)stdlib.c $(LIBCINC)stdlib.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
string.o: $(LIBC)string.c $(LIBCINC)string.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
math.o: $(LIBC)math.c $(LIBCINC)math.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra

sysdep.o: $(LIBC)sysdep.c $(LIBCINC)sysdep.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra


########################################### task

tasks.o: $(TASKDIR)task.s
	$(CCAS) $< -o tasks.o

task.o: $(TASKDIR)task.c $(INCLUDEDIR)task.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra



########################################### kernel

interruptas.o: $(KRNDIR)interrupt.s
	$(CCAS) $< -o interruptas.o

interrupt.o: $(KRNDIR)interrupt.c $(INCLUDEDIR)interrupt.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra
 
kernel.o: $(KRNDIR)kernel.c $(INCLUDEDIR)kernel.h $(INCLUDEDIR)multiboot.h
	$(CC) -c $^ -std=gnu99 $(CFLAGS) -Wextra



############################################

create: $(OBJAS) $(OBJ)
	$(CC) -T linker.ld -o nkos.bin $(CFLAGS) -nostdlib *.o -lgcc
	grub-file --is-x86-multiboot nkos.bin
	\cp -f nkos.bin isodir/boot/nkos.bin
	grub-mkrescue -o nkos.iso isodir

clean:
	rm -f *.o nkos.bin isodir/boot/nkos.bin  $(INCLUDEDIR)*.gch $(INCLUDEDIR)*~ ./*~ $(ARCHDIR)/*.o $(DRVDIR)/*.o $(KRDIR)/*.o $(LIBC)/*.o $(LIBCINC)/*.gch $(MEMDIR)/*.o



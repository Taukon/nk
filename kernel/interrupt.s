.global as_keyboard_interrupt
.global as_timer_interrupt            
.global as_software_interrupt  


.extern keyboard_interrupt
.extern timer_interrupt    
.extern syscall_interrupt 

as_keyboard_interrupt:
        push    %gs
        push    %fs
        push    %es
        push    %ds
        pushal

        mov     $0x10, %ax
        mov 	%ax,   %ds
	mov 	%ax,   %es
	mov 	%ax,   %fs
	mov 	%ax,   %gs

        call    keyboard_interrupt

        popal
        pop     %ds
        pop     %es
        pop     %fs
        pop     %gs

        sti
        iretl

as_timer_interrupt:
        push    %gs
        push    %fs
        push    %es
        push    %ds
        pushal

        mov     $0x10, %ax
        mov 	%ax,   %ds
	mov 	%ax,   %es
	mov 	%ax,   %fs
	mov 	%ax,   %gs

        call    timer_interrupt
 
        popal
        pop     %ds
        pop     %es
        pop     %fs
        pop     %gs

        sti
        iretl


as_software_interrupt:
	pushl   %esp
	pushl   %ebp
	pushl   %esi
	pushl   %edi
	pushl   %edx
	pushl   %ecx
	pushl   %ebx
	pushl   %eax
	call    syscall_interrupt	
	popl    %eax
	popl    %ebx
	popl    %ecx
	popl    %edx
	popl    %edi
	popl    %esi
	popl    %ebp
	popl    %esp
	iretl
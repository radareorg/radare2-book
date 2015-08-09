## Assembler

`rasm2` can be used from the command-line to quickly copy-paste hexpairs that represent a given machine instruction.

    $ rasm2 -a x86 -b 32 'mov eax, 33'
    b821000000

    $ echo 'push eax;nop;nop' | rasm2 -f -
    5090

Rasm2 is used by radare2 core to write bytes using `wa` command.

The assembler understands the following input languages and their flavors: x86 (Intel and AT&T variants), olly (OllyDBG syntax), powerpc (PowerPC), arm and java. For Intel syntax, rasm2 tries to mimic NASM or GAS.

There are several examples in the rasm2 source code directory. Consult them to understand how you can assemble a raw binary file from a rasm2 description.

    $ cat selfstop.rasm
    ;
    ; Self-Stop shellcode written in rasm for x86
    ;
    ; --pancake
    ;
    
    .arch x86
    .equ base 0x8048000
    .org 0x8048000  ; the offset where we inject the 5 byte jmp
    
    selfstop:
      push 0x8048000
      pusha
      mov eax, 20
      int 0x80
    
      mov ebx, eax 
      mov ecx, 19
      mov eax, 37
      int 0x80
      popa
      ret
    ;
    ; The call injection
    ;
    
      ret
      
    [0x00000000]> e asm.bits = 32
    [0x00000000]> wx `!rasm2 -f a.rasm`
    [0x00000000]> pd 20
           0x00000000    6800800408   push 0x8048000 ;  0x08048000 
           0x00000005    60           pushad
           0x00000006    b814000000   mov eax, 0x14 ;  0x00000014 
           0x0000000b    cd80         int 0x80
              syscall[0x80][0]=?
           0x0000000d    89c3         mov ebx, eax
           0x0000000f    b913000000   mov ecx, 0x13 ;  0x00000013 
           0x00000014    b825000000   mov eax, 0x25 ;  0x00000025 
           0x00000019    cd80         int 0x80
              syscall[0x80][0]=?
           0x0000001b    61           popad
           0x0000001c    c3           ret
           0x0000001d    c3           ret

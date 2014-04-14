#Chapter 6: Disassembling

Disassembling in radare is just a way to represent a bunch of bytes. So it is handled as a print mode with the 'p' command.

In the old times when radare core was smaller. The disassembler was handled by an external rsc file, so radare was dumping the current block into a file, and the script was just calling objdump in a proper way to disassemble for intel, arm, etc...

Obviously this is a working solution, but takes too much cpu for repeating just the same task so many times, because there are no caches and the scrolling was absolutely slow.

Nowadays, the disassembler is one of the basics in radare allowing you to choose the architecture flavour and some To disassemble use the 'pd' command.

The 'pd' command accepts a numeric argument to specify how many opcodes of the current block do you want to disassemble. Most of the commands in radare are restricted by the block size. So if you want to disassemble more bytes you should use the 'b' command to specify the new block size.

    [0x00000000]> b 100    ; set block size to 100
    [0x00000000]> pd       ; disassemble 100 bytes
    [0x00000000]> pd 3     ; disassemble 3 opcodes
    [0x00000000]> pD 30    ; disassemble 30 bytes

The 'pD' command works like 'pd' but gets the number of bytes instead of the number of opcodes.

The 'pseudo' syntax is closer to the humans, but it can be anoying if you are reading lot of code:

    [0xB7FB8810]> e asm.pseudo = true
    [0xB7FB8810]> pd 3
    0x00404888    31ed         ebp = 0
    0x0040488a    4989d1       r9 = rdx
    0x0040488d    5e           pop rsi   

    [0xB7FB8810]> e asm.syntax=intel
    [0xB7FB8810]> pd 3
    0xB7FB8810,  mov eax, esp        
    0xB7FB8812   call 0xb7fb8a60
    0xB7FB8817   add edi, eax        

    [0xB7FB8810]> e asm.syntax=att
    [0xB7FB8810]> pd 3
    0xB7FB8810,  mov %esp, %eax          
    0xB7FB8812   call 0xb7fb8a60
    0xB7FB8817   add %eax, %edi          
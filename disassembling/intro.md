# Disassembling

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

    [0x00405e1c]> e asm.pseudo = true
    [0x00405e1c]> pd 3
              ; JMP XREF from 0x00405dfa (fcn.00404531)
              0x00405e1c    488b9424a80. rdx = [rsp+0x2a8]
              0x00405e24    64483314252. rdx ^= [fs:0x28]
              0x00405e2d    4889d8       rax = rbx
    
    [0x00405e1c]> e asm.syntax = intel
    [0x00405e1c]> pd 3
              ; JMP XREF from 0x00405dfa (fcn.00404531)
              0x00405e1c    488b9424a80. mov rdx, [rsp+0x2a8]
              0x00405e24    64483314252. xor rdx, [fs:0x28]
              0x00405e2d    4889d8       mov rax, rbx
    
    [0x00405e1c]> e asm.syntax=att
    [0x00405e1c]> pd 3
              ; JMP XREF from 0x00405dfa (fcn.00404531)
              0x00405e1c    488b9424a80. mov 0x2a8(%rsp), %rdx
              0x00405e24    64483314252. xor %fs:0x28, %rdx
              0x00405e2d    4889d8       mov %rbx, %rax

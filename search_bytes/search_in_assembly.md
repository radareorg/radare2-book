## Assembler Search

If you want to search for a certain assembler opcodes, you can either use `/c` or `/a` commands.

*    `/c jmp [esp]` - search for specified asm mnemonics

    [0x00404888]> /c jmp qword [rdx]
    f hit_0 @ 0x0040e50d   # 2: jmp qword [rdx]
    f hit_1 @ 0x00418dbb   # 2: jmp qword [rdx]
    f hit_2 @ 0x00418fcb   # 3: jmp qword [rdx]
    f hit_3 @ 0x004196ab   # 6: jmp qword [rdx]
    f hit_4 @ 0x00419bf3   # 3: jmp qword [rdx]
    f hit_5 @ 0x00419c1b   # 3: jmp qword [rdx]
    f hit_6 @ 0x00419c43   # 3: jmp qword [rdx]

* `/a jmp eax` - assemble string to machine code, and then search for resulting bytes.

    [0x00404888]> /a jmp eax
    hits: 1
    0x004048e7 hit3_0 ffe00f1f8000000000b8

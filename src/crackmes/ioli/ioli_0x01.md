IOLI 0x01
=========

This is the second IOLI crackme.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: test
Invalid Password!
```

Let's check for strings with rabin2.

```
$ rabin2 -z ./crackme0x01
[Strings]
nth paddr      vaddr      len size section type  string
―――――――――――――――――――――――――――――――――――――――――――――――――――――――
0   0x00000528 0x08048528 24  25   .rodata ascii IOLI Crackme Level 0x01\n
1   0x00000541 0x08048541 10  11   .rodata ascii Password: 
2   0x0000054f 0x0804854f 18  19   .rodata ascii Invalid Password!\n
3   0x00000562 0x08048562 15  16   .rodata ascii Password OK :)\n
```

This isn't going to be as easy as 0x00. Let's try disassembly with r2.

```
$ r2 ./crackme0x01 
-- Use `zoom.byte=printable` in zoom mode ('z' in Visual mode) to find strings
[0x08048330]> aa
[0x08048330]> pdf@main
            ; DATA XREF from entry0 @ 0x8048347
/ 113: int main (int argc, char **argv, char **envp);
|           ; var int32_t var_4h @ ebp-0x4
|           ; var int32_t var_sp_4h @ esp+0x4
|           0x080483e4      55             push ebp
|           0x080483e5      89e5           mov ebp, esp
|           0x080483e7      83ec18         sub esp, 0x18
|           0x080483ea      83e4f0         and esp, 0xfffffff0
|           0x080483ed      b800000000     mov eax, 0
|           0x080483f2      83c00f         add eax, 0xf                ; 15
|           0x080483f5      83c00f         add eax, 0xf                ; 15
|           0x080483f8      c1e804         shr eax, 4
|           0x080483fb      c1e004         shl eax, 4
|           0x080483fe      29c4           sub esp, eax
|           0x08048400      c70424288504.  mov dword [esp], str.IOLI_Crackme_Level_0x01 ; [0x8048528:4]=0x494c4f49 ; "IOLI Crackme Level 0x01\n"
|           0x08048407      e810ffffff     call sym.imp.printf         ; int printf(const char *format)
|           0x0804840c      c70424418504.  mov dword [esp], str.Password: ; [0x8048541:4]=0x73736150 ; "Password: "
|           0x08048413      e804ffffff     call sym.imp.printf         ; int printf(const char *format)
|           0x08048418      8d45fc         lea eax, [var_4h]
|           0x0804841b      89442404       mov dword [var_sp_4h], eax
|           0x0804841f      c704244c8504.  mov dword [esp], 0x804854c  ; [0x804854c:4]=0x49006425
|           0x08048426      e8e1feffff     call sym.imp.scanf          ; int scanf(const char *format)
|           0x0804842b      817dfc9a1400.  cmp dword [var_4h], 0x149a
|       ,=< 0x08048432      740e           je 0x8048442
|       |   0x08048434      c704244f8504.  mov dword [esp], str.Invalid_Password ; [0x804854f:4]=0x61766e49 ; "Invalid Password!\n"
|       |   0x0804843b      e8dcfeffff     call sym.imp.printf         ; int printf(const char *format)
|      ,==< 0x08048440      eb0c           jmp 0x804844e
|      |`-> 0x08048442      c70424628504.  mov dword [esp], str.Password_OK_: ; [0x8048562:4]=0x73736150 ; "Password OK :)\n"
|      |    0x08048449      e8cefeffff     call sym.imp.printf         ; int printf(const char *format)
|      |    ; CODE XREF from main @ 0x8048440
|      `--> 0x0804844e      b800000000     mov eax, 0
|           0x08048453      c9             leave
\           0x08048454      c3             ret
```

"aa" tells r2 to analyze the whole binary, which gets you symbol names, among things.

"pdf" stands for

*	Print

*	Disassemble

*	Function

This will print the disassembly of the main function, or the `main()` that everyone knows. You can see several things as well: weird names, arrows, etc.

*	"imp." stands for imports. Those are imported symbols, like printf()

*	"str." stands for strings. Those are strings (obviously).

If you look carefully, you'll see a `cmp` instruction, with a constant, 0x149a. `cmp` is an x86 compare instruction, and the 0x in front of it specifies it is in base 16, or hex (hexadecimal).

```
0x0804842b    817dfc9a140. cmp dword [ebp + 0xfffffffc], 0x149a
```

You can use radare2's `?` command to display 0x149a in another numeric base.

```
[0x08048330]> ? 0x149a
int32   5274
uint32  5274
hex     0x149a
octal   012232
unit    5.2K
segment 0000:049a
string  "\x9a\x14"
fvalue: 5274.0
float:  0.000000f
double: 0.000000
binary  0b0001010010011010
trits   0t21020100
```

So now we know that 0x149a is 5274 in decimal. Let's try this as a password.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: 5274
Password OK :)
```

Bingo, the password was 5274. In this case, the password function at 0x0804842b was comparing the input against the value, 0x149a in hex. Since user input is usually decimal, it was a safe bet that the input was intended to be in decimal, or 5274. Now, since we're hackers, and curiosity drives us, let's see what happens when we input in hex.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: 0x149a
Invalid Password!
```

It was worth a shot, but it doesn't work. That's because `scanf()` will take the 0 in 0x149a to be a zero, rather than accepting the input as actually being the hex value.

And this concludes IOLI 0x01.

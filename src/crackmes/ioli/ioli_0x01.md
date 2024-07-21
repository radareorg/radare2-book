### IOLI 0x01

This second challenge is designed to introduce you to more advanced reverse engineering techniques using Radare2. The objective is to find the correct password to unlock the program by examining the binary’s disassembly.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: test
Invalid Password!
```

#### Hints

To solve this challenge, this time you will need to go beyond searching for plaintext strings.

Learn how to load the binary with radare2, analise the code and disassemble the main function.

* Load the binary with radare2 and use the `-A` or `-AA` flags to analyze the program before showing the prompt.

These flags will run `aa` or `aaa`. The more a's you append the deeper the analysis will be, so it will perform more actions, which in some cases it's useful, but in other can result on invalid results, learn about the differences and find the right balance for each target you face. For our needs `aa` should be probably enough.

```console
$ r2 -A crackme0x01
```

To disassemble the main function you can use the `pdf` command. You can learn about other disassembly commands by typing `pd?`.

In the disassembled code, look for cmp (compare) instructions. These are often used to compare user input against hardcoded values. Identifying these values can help you find the correct password.

You can practice a little the `|` (pipe operator) or the `~` (internal grep) special characters to `grep` directly the instructions you need:

```console
> s main
> pdf~cmp
```

Usually the immediate values displayed in the disassembly are formatted in hexadecimal. Use the rax2 program or the `?` command to find out the representation in other bases (like base10)

```console
$ rax2 0x123
291
```

Now it's probably a good time to make another blind guess trying the value by running the crackme and typing the number.

#### Solution

Let's go step by step to solve the second IOLI crackme. We can start by trying what we learned in the previous challenge by listing the strings with `rabin2`:

```
$ rabin2 -z ./crackme0x01
[Strings]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000528 0x08048528 24  25   .rodata ascii IOLI Crackme Level 0x01\n
1   0x00000541 0x08048541 10  11   .rodata ascii Password: 
2   0x0000054f 0x0804854f 18  19   .rodata ascii Invalid Password!\n
3   0x00000562 0x08048562 15  16   .rodata ascii Password OK :)\n
```

This isn't going to be as easy as 0x00. Let's try disassembly with r2.

```
$ r2 ./crackme0x01 
[0x08048330]> aa
INFO: Analyze all flags starting with sym. and entry0 (aa)
INFO: Analyze imports (af@@@i)
INFO: Analyze entrypoint (af@ entry0)
INFO: Analyze symbols (af@@@s)
INFO: Recovering variables (afva@@@F)
INFO: Analyze all functions arguments/locals (afva@@@F)
[0x08048330]> -e asm.bytes=false  # dont show the bytes
[0x08048330]> pdf@main
       ; DATA XREF from entry0 @ 0x8048347
/ 113: int main (int argc, char **argv, char **envp);
|      ; var int32_t var_4h @ ebp-0x4
|      ; var int32_t var_sp_4h @ esp+0x4
|      0x080483e4    push ebp
|      0x080483e5    mov ebp, esp
|      0x080483e7    sub esp, 0x18
|      0x080483ea    and esp, 0xfffffff0
|      0x080483ed    mov eax, 0
|      0x080483f2    add eax, 0xf
|      0x080483f5    add eax, 0xf
|      0x080483f8    shr eax, 4
|      0x080483fb    shl eax, 4
|      0x080483fe    sub esp, eax
|      0x08048400    mov dword [esp], str.IOLI_Crackme_Level_0x01
|      0x08048407    call sym.imp.printf
|      0x0804840c    mov dword [esp], str.Password:
|      0x08048413    call sym.imp.printf
|      0x08048418    lea eax, [var_4h]
|      0x0804841b    mov dword [var_sp_4h], eax
|      0x0804841f    mov dword [esp], 0x804854c
|      0x08048426    call sym.imp.scanf
|      0x0804842b    cmp dword [var_4h], 0x149a
|  ,=< 0x08048432    je 0x8048442
|  |   0x08048434    mov dword [esp], str.Invalid_Password
|  |   0x0804843b    call sym.imp.printf
| ,==< 0x08048440    jmp 0x804844e
| |`-> 0x08048442    mov dword [esp], str.Password_OK_:
| |    0x08048449    call sym.imp.printf
| `--> 0x0804844e    mov eax, 0
|      0x08048453    leave
\      0x08048454    ret
```

The `aa` command instructs r2 to analyze the whole binary, which gets you symbol names, among things.

The `pdf` stands for "Print" "Disassembly" of the "Function". The `@` character will perform a temporal seek to the given address or symbol name.

This will print the disassembly of the main function, or the `main()` that everyone knows. You can see several things as well: weird names, arrows, etc.

* `imp.` stands for imports. (Functions imported from libraries, like `printf` which is in the libc)
* `str.` stands for strings. (Usually those listed by the `iz` command)

If you look carefully, you'll see a `cmp` instruction, with a constant, 0x149a. `cmp` is an x86 compare instruction, and the 0x in front of it specifies it is in base 16, or hex (hexadecimal).

```
[0x08048330]> pdf@main~cmp
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

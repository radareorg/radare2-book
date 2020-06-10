# ragg2

ragg2 stands for `radare2 egg`, this is the basic block to construct relocatable
snippets of code to be used for injection in target processes when doing exploiting.

ragg2 compiles programs written in a simple high-level language into tiny binaries
for x86, x86-64, and ARM.

By default it will compile it's own `ragg2` language, but you can also compile C
code using GCC or Clang shellcodes depending on the file extension. Lets create
C file called `a.c`:
```c
int main() {
	write(1, "Hello World\n", 13);
	return 0;
}
```
```
$ ragg2 -a x86 -b32 a.c
e900000000488d3516000000bf01000000b80400000248c7c20d0000000f0531c0c348656c6c6f20576f726c640a00

$ rasm2 -a x86 -b 32 -D e900000000488d3516000000bf01000000b80400000248c7c20d0000000f0531c0c348656c6c6f20576f726c640a00
0x00000000   5               e900000000  jmp 5
0x00000005   1                       48  dec eax
0x00000006   6             8d3516000000  lea esi, [0x16]
0x0000000c   5               bf01000000  mov edi, 1
0x00000011   5               b804000002  mov eax, 0x2000004
0x00000016   1                       48  dec eax
0x00000017   6             c7c20d000000  mov edx, 0xd
0x0000001d   2                     0f05  syscall
0x0000001f   2                     31c0  xor eax, eax
0x00000021   1                       c3  ret
0x00000022   1                       48  dec eax
0x00000023   2                     656c  insb byte es:[edi], dx
0x00000025   1                       6c  insb byte es:[edi], dx
0x00000026   1                       6f  outsd dx, dword [esi]
0x00000027   3                   20576f  and byte [edi + 0x6f], dl
0x0000002a   2                     726c  jb 0x98
0x0000002c   3                   640a00  or al, byte fs:[eax]
```

## Compiling ragg2 example

```
$ cat hello.r
exit@syscall(1);

main@global() {
	exit(2);
}

$ ragg2 -a x86 -b 64 hello.r
48c7c00200000050488b3c2448c7c0010000000f054883c408c3
0x00000000   1                       48  dec eax
0x00000001   6             c7c002000000  mov eax, 2
0x00000007   1                       50  push eax
0x00000008   1                       48  dec eax
0x00000009   3                   8b3c24  mov edi, dword [esp]
0x0000000c   1                       48  dec eax
0x0000000d   6             c7c001000000  mov eax, 1
0x00000013   2                     0f05  syscall
0x00000015   1                       48  dec eax
0x00000016   3                   83c408  add esp, 8
0x00000019   1                       c3  ret

$ rasm2 -a x86 -b 64 -D 48c7c00200000050488b3c2448c7c0010000000f054883c408c3
0x00000000   7           48c7c002000000  mov rax, 2
0x00000007   1                       50  push rax
0x00000008   4                 488b3c24  mov rdi, qword [rsp]
0x0000000c   7           48c7c001000000  mov rax, 1
0x00000013   2                     0f05  syscall
0x00000015   4                 4883c408  add rsp, 8
0x00000019   1                       c3  ret
```

## Tiny binaries

You can create them using the `-F` flag in ragg2, or the `-C` in rabin2.


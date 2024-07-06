### IOLI 0x00

This is the first IOLI crackme, and the easiest one.

```
$ ./crackme0x00
IOLI Crackme Level 0x00
Password: 1234
Invalid Password!
```

The first thing to check is if the password is just plaintext inside the file. In this case, we don't need to do any disassembly, and we can just use rabin2 with the -z flag to search for strings in the binary.

```
$ rabin2 -z ./crackme0x00
[Strings]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000568 0x08048568 24  25   .rodata ascii IOLI Crackme Level 0x00\n
1   0x00000581 0x08048581 10  11   .rodata ascii Password: 
2   0x0000058f 0x0804858f 6   7    .rodata ascii 250382
3   0x00000596 0x08048596 18  19   .rodata ascii Invalid Password!\n
4   0x000005a9 0x080485a9 15  16   .rodata ascii Password OK :)\n
```

So we know what the following section is, this section is the header shown when the application is run.

```
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000568 0x08048568 24  25   .rodata ascii IOLI Crackme Level 0x00\n
```

Here we have the prompt for the password.

```
1   0x00000581 0x08048581 10  11   .rodata ascii Password: 
```

This is the error on entering an invalid password.

```
3   0x00000596 0x08048596 18  19   .rodata ascii Invalid Password!\n
```

This is the message on the password being accepted.

```
4   0x000005a9 0x080485a9 15  16   .rodata ascii Password OK :)\n
```

What is this? It's a string, but we haven't seen it in running the application yet.

```
2   0x0000058f 0x0804858f 6   7    .rodata ascii 250382
```

Let's give this a shot.

```
$ ./crackme0x00
IOLI Crackme Level 0x00
Password: 250382
Password OK :)
```

So we now know that 250382 is the password, and have completed this crackme.

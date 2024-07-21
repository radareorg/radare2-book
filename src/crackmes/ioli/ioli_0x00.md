### IOLI 0x00

This first challenge is designed to introduce you to the basics of reverse engineering with Radare2. The objective is to find the correct password to unlock the program.

By executing the program you may see something like this:

```
$ ./crackme0x00
IOLI Crackme Level 0x00
Password: 1234
Invalid Password!
```

#### Hints

For this initial challenge, you won’t need to dive into complex disassembly. Instead, focus on searching for plaintext strings within the binary file.

There are multiple ways to find the strings embedded inside a binary, which are equivalents to the GNU `strings` utility.

* Check the manpage and help message of rabin2
  * Use `man rabin2` and `rabin2 -h` in your terminal
* Load the binary with radare2
  * Append the question mark to the `i` and `iz` commands to find relevant 
  * Understand the difference between `iz`, `izz` and `izzz`
* Read the output of those commands and make a blind guess

#### Solution

As explained in the hints, the first thing to check is if the password is just plaintext inside the file. In this case, we don't need to do any disassembly, and we can just use rabin2 with the -z flag to search for strings in the binary.

```console
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

Let's understand the output of this command line by line:

The first section is the header displayed when the application runs.

```console
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000568 0x08048568 24  25   .rodata ascii IOLI Crackme Level 0x00\n
```

Next, we see the prompt for the password.

```
1   0x00000581 0x08048581 10  11   .rodata ascii Password: 
```

Then, the error message for an invalid password.

```
3   0x00000596 0x08048596 18  19   .rodata ascii Invalid Password!\n
```

Finally, the message indicating a successful password entry.

```
4   0x000005a9 0x080485a9 15  16   .rodata ascii Password OK :)\n
```

What about this string? It hasn’t appeared when running the application yet.

```
2   0x0000058f 0x0804858f 6   7    .rodata ascii 250382
```

Let’s try using it as the password.

```
$ ./crackme0x00
IOLI Crackme Level 0x00
Password: 250382
Password OK :)
```

Now we know that 250382 is the correct password, completing this crackme!

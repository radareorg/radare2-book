# Testing the plugin

This plugin is used by rasm2 and r2. You can verify that the plugin is properly loaded with this command:
```
$ rasm2 -L | grep mycpu
_d  mycpu        My CPU disassembler  (LGPL3)
```

Let's open an empty file using the 'mycpu' arch and write some random code there.

```
$ r2 -
 -- I endians swap
[0x00000000]> e asm.arch=mycpu
[0x00000000]> woR
[0x00000000]> pd 10
           0x00000000    888e         mov r8, 14
           0x00000002    b2a5         ifnot r10, r5
           0x00000004    3f67         ret
           0x00000006    7ef6         bl r15, r6
           0x00000008    2701         xor r0, 1
           0x0000000a    9826         mov r2, 6
           0x0000000c    478d         xor r8, 13
           0x0000000e    6b6b         store r6, 11
           0x00000010    1382         add r8, r2
           0x00000012    7f15         ret
```
Yay! it works.. and the mandatory oneliner too!

```
r2 -nqamycpu -cwoR -cpd' 10' -
```


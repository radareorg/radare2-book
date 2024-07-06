### IOLI 0x08

we can reverse it and find it's similar to 0x07, and use the same password to solve it:

```sh
$ export LOLAA=help
$ ./cracke0x08
IOLI Crackme Level 0x08
Password: 12346
Password OK!
```

[dustri](https://dustri.org/b/defeating-ioli-with-radare2.html) provided a better way to check crackme0x08. 0x07 is the stripped version of 0x08.

```sh
$ radiff2 -A -C ./crackme0x07 ./crackme0x08
...
              fcn.08048360  23 0x8048360 |   MATCH  (1.000000) | 0x8048360   23 sym._init
 sym.imp.__libc_start_main   6 0x8048388 |   MATCH  (1.000000) | 0x8048388    6 sym.imp.__libc_start_main
             sym.imp.scanf   6 0x8048398 |   MATCH  (1.000000) | 0x8048398    6 sym.imp.scanf
            sym.imp.strlen   6 0x80483a8 |   MATCH  (1.000000) | 0x80483a8    6 sym.imp.strlen
            sym.imp.printf   6 0x80483b8 |   MATCH  (1.000000) | 0x80483b8    6 sym.imp.printf
            sym.imp.sscanf   6 0x80483c8 |   MATCH  (1.000000) | 0x80483c8    6 sym.imp.sscanf
           sym.imp.strncmp   6 0x80483d8 |   MATCH  (1.000000) | 0x80483d8    6 sym.imp.strncmp
              sym.imp.exit   6 0x80483e8 |   MATCH  (1.000000) | 0x80483e8    6 sym.imp.exit
                    entry0  33 0x8048400 |   MATCH  (1.000000) | 0x8048400   33 entry0
              fcn.08048424  33 0x8048424 |   MATCH  (1.000000) | 0x8048424   33 fcn.08048424
              fcn.08048450  47 0x8048450 |   MATCH  (1.000000) | 0x8048450   47 sym.__do_global_dtors_aux
              fcn.08048480  50 0x8048480 |   MATCH  (1.000000) | 0x8048480   50 sym.frame_dummy
              fcn.080484b4 112 0x80484b4 |   MATCH  (1.000000) | 0x80484b4  112 sym.dummy
              fcn.08048524  30 0x8048524 |   MATCH  (1.000000) | 0x8048524   30 sym.che
              fcn.08048542 119 0x8048542 |   MATCH  (1.000000) | 0x8048542  119 sym.parell
              fcn.080485b9 118 0x80485b9 |   MATCH  (1.000000) | 0x80485b9  118 sym.check
                      main  92 0x804867d |   MATCH  (1.000000) | 0x804867d   92 main
              fcn.08048755   4 0x8048755 |   MATCH  (1.000000) | 0x8048755    4 sym.__i686.get_pc_thunk.bx
              fcn.08048760  35 0x8048760 |   MATCH  (1.000000) | 0x8048760   35 sym.__do_global_ctors_aux
              fcn.0804878d  17 0x804878d |     NEW  (0.000000)
       sym.__libc_csu_init  99 0x80486e0 |     NEW  (0.000000)
       sym.__libc_csu_fini   5 0x8048750 |     NEW  (0.000000)
                 sym._fini  26 0x8048784 |     NEW  (0.000000)
```

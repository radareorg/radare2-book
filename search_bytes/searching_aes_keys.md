## Searching for AES Keys

Thanks to Victor Muñoz, radare2 now has support of the algorithm he developed, capable of finding expanded AES keys with `/ca` command. It searches from current seek position up to the `search.distance` limit, or until end of file is reached. You can interrupt current search by pressing `Ctrl-C`. For example, to look for AES keys in physical memory of your system:
```
$ sudo r2 /dev/mem
[0x00000000]> /ca
0 AES keys found
```

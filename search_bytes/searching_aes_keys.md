## Searching for AES Keys

Thanks to Victor Muñoz, radare2 now has support of the algorithm he developed, capable of finding **expanded AES** keys with `/Ca` command. It searches from current seek position up to the `search.distance` limit, or until end of file is reached. You can interrupt current search by pressing `Ctrl-C`. For example, to look for AES keys in physical memory of your system:

```
$ sudo r2 /dev/mem
[0x00000000]> /Ca
0 AES keys found
```

If you are simply looking for plaintext AES keys in your binary, radare2 will point to them as symbols so `/Ca` will not find the AES key, but `is~AES` should conveniently point to it instead:

```
[0x00000000]> /Ca
Searching 0 byte in [0x100000-0x1f0000]
hits: 0
Searching 0 byte in [0x196e4-0x1b91c]
hits: 0
Searching 0 byte in [0x194b4-0x196e4]
hits: 0
Searching 0 byte in [0x8000-0x114b4]
hits: 0
[0x00000000]> is~AES
010 0x000096d4 0x000196d4 GLOBAL    OBJ   16 AES_KEY
```

## Searching for AES Keys

Thanks to Victor Muñoz, radare2 now has support of the algorithm he developed, capable of finding **expanded AES** keys with `/Ca` command. It searches from current seek position up to the `search.distance` limit, or until end of file is reached. You can interrupt current search by pressing `Ctrl-C`. For example, to look for AES keys in physical memory of your system:

```
$ sudo r2 /dev/mem
[0x00000000]> /Ca
0 AES keys found
```

If you are simply looking for plaintext AES keys in your binary, `/Ca` will not find them, but you might want to search with `is~AES` instead if the programmer left those hints for you:

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

Other than that, AES keys might show up in different ways in the binary: encrypted, hidden by another encrypting routine, so there's no absolute way other than understanding the binary being analized. For instance, `p=e` might give some hints if high(er) entropy sections are found trying to cover up a hardcoded secret. Moreover AES keys might be referenced from strings or pointed from the imports, for instance, so the `/` and other search-related commands can come in handy in this regard.

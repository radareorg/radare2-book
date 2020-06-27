## Searching for AES Keys

Thanks to Victor Muñoz, radare2 now has support of the algorithm he developed, capable of finding **expanded AES** keys with `/Ca` command. It searches from current seek position up to the `search.distance` limit, or until end of file is reached. You can interrupt current search by pressing `Ctrl-C`. For example, to look for AES keys in physical memory of your system:

```
$ sudo r2 /dev/mem
[0x00000000]> /ca
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

Other than that, AES keys might show up in different ways in the binary: encrypted, hidden by another encrypting routine, so there's no absolute way other than understanding the binary being analized. For instance, `p=e` might give some hints if high(er) entropy sections are found trying to cover up a hardcoded secret. As an example on entropy searching, since radare 3.2.0, there's the possibility to delimit entropy sections for later use like so:

```
[0x00000000]> b
0x100
[0x00000000]> b 4096
[0x00000000]> /s
0x00100000 - 0x00101000 ~ 5.556094
0x014e2c88 - 0x014e3c88 ~ 0.000000
0x01434374 - 0x01435374 ~ 6.332087
0x01435374 - 0x0144c374 ~ 3.664636
0x0144c374 - 0x0144d374 ~ 1.664368
0x0144d374 - 0x0144f374 ~ 4.229199
0x0144f374 - 0x01451374 ~ 2.000000
(...)
[0x00000000]> /s*
f entropy_section_0 0x00001000 0x00100000
f entropy_section_1 0x00001000 0x014e2c88
f entropy_section_2 0x00001000 0x01434374
f entropy_section_3 0x00017000 0x01435374
f entropy_section_4 0x00001000 0x0144c374
f entropy_section_5 0x00002000 0x0144d374
f entropy_section_6 0x00002000 0x0144f374
```

The blocksize is increased to 4096 bytes from the default 100 bytes so that the entropy search `/s` can work on reasonably sized chunks for entropy analysis. The sections flags can be applied with the dot operator, `./s*` and then looped through `px 32 @@ entropy*`.

Moreover AES keys might be referenced from strings or pointed from the imports, for instance, so the `/` and other search-related commands can come in handy in this regard.

## Searching for Cryptography materials

### Searching expanded keys
radare2 is capable of finding **expanded** keys with `/ca` command for AES and SM4 block ciphers. It searches from current seek position up to the `search.distance` limit, or until end of file is reached. You can interrupt current search by pressing `Ctrl-C`. For example, to look for AES keys in a memory dump:

```
0x00000000]> /ca aes
Searching 40 bytes in [0x0-0x1ab]
hits: 1
0x000000fb hit0_0 6920e299a5202a6d656e636869746f2a
```

For AES, the output length gives you the size of the AES key used: 128, 192 or 256 bits. If you are simply looking for plaintext AES keys in your binary, `/ca` will not find them they must have been expanded by the key expansion algorithm.

### Searching private keys and certificates
`/cr` command implements the search of private keys (RSA and ECC). `/cd` command implements a similar feature to search certificates.

```
[0x00000000]> /cr
Searching 11 bytes in [0x0-0x15a]
hits: 2
0x000000fa hit1_0 302e020100300506032b657004220420fb3d588296fed5694ff7049eafb74490bf4bc6467ee11a08...
```

### Entropy analysis
`p=e` might give some hints if high entropy sections are found trying to cover up a hardcoded secret. 

There is the possibility to delimit entropy sections for later use with `\s` command:

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

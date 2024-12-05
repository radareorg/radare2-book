### Encoding with rahash2

As mentioned before, this tool also provide the tooling needed to encode and decode between different encodings

```console
$ rahash2 -L | grep ^e
e  base64
e  base91
e  punycode
```

For example, to encode a string into base64 use the following line:

```console
$ rahash2 -E base64 -s hello
```

You can decode it by using the -D flag instead of -E.

### Encryption/Decryption

For encrypting data check the crypto hash plugins:

```console
$ rahash2 -L | grep "^c"                                                       ✔ 
c      aes-ecb  Rijndael block cipher with Electronic Code Book mode
c      aes-cbc  Rijndael block cipher with Cipher Block Chaining mode
c     aes-wrap  Rijndael block cipher with Key Wrap Algorithm (RFC 3394)
c     blowfish  Bruce Schneier's symetric-key block cipher
c         cps2  Capcom Play System 2
c      des-ecb  DES with with Electronic Code Book mode
c          rc2  Ron Rivest's Code symmetric key encryption also known as ARC2
c          rc4  Rivest Cipher 4
c          rc6  Rivest's Cipher 6
c  serpent-ecb  Serpent block cipher with Electronic Code Book mode
c      sm4-ecb  ShāngMì4 block cipher with Electronic Code Book mode
c          xor  Byte level Exclusive Or
```

Here's an example usage to encrypt a string using rahash2:

```console
$ rahash2 -E xor -S s:password -s hello | hexdump -C
00000000  18 04 1f 1f 18
```

## Shellcode Encoders

ragg2 offers a few ready-made shellcodes and encoders.

```sh
$ ragg2 -L
shellcodes:
      exec : execute cmd=/bin/sh suid=false
encoders:
       xor : xor encoder for shellcode
```

Using the '-i' option, one can generate specify and generate the shellcode.

```sh
$ ragg2 -i exec
31c048bbd19d9691d08c97ff48f7db53545f995257545eb03b0f05
```

Similar to the previous section, the output format(c, raw, elf etc.,) can be specified here too along with the architecture and bits.

ragg2 offers an xor encoder too. The following are the relevant flags/options.

```sh
$ ragg2 -h
 -c [k=v]        set configuration options
 -E [encoder]    use specific encoder. see -L
 -L              list all plugins (shellcodes and encoders)
```

```sh
$ ragg2 -E xor -c key=32 -i exec
6a1b596a205be8ffffffffc15e4883c60d301e48ffc6e2f911e0689bf1bdb6b1f0acb7df68d7fb73747fb97277747e901b2f25
```

The same can be done with a .c or .r file output. The first one is the normal output(machine code) and the second is xor encoded.

```sh
$ ragg2 -a x86 -f raw code1.c
eb0e66666666662e0f1f84000000000050bf01000000488d359f000000ba0d000000e81900000031ff89442404e85e00000031d289042489d059c30f1f440000897c24fc48897424f0895424ec8b5424fc895424dc488b7424f048897424d08b5424ec895424cc8b7c24dc488b7424d08b5424ccb8010000000f0548894424e0488b4424e089c1894c24c88b4424c8c3897c24fc8b7c24fc897c24ec8b7c24ecb83c0000000f0548894424f0488b4424f089c1894c24e88b4424e8c348656c6c6f20576f726c640a00

$ ragg2 -E xor -c key=127 -a x86 -f raw code1.c
6ac9596a7f5be8ffffffffc15e4883c60d301e48ffc6e2f994711919191919517060fb7f7f7f7f7f2fc07e7f7f7f37f24ae07f7f7fc5727f7f7f97667f7f7f4e80f63b5b7b97217f7f7f4eadf67b5bf6af26bc70603b7f7ff6035b8337f60b5b8ff62b5b93f42b5b83f62b5ba337f40b5b8f37f60b5baff42b5b93f62b5bb3f4035ba337f40b5baff42b5bb3c77e7f7f7f707a37f63b5b9f37f43b5b9ff6bef6335bb7f43b5bb7bcf6035b83f4035b83f6035b93f4035b93c7437f7f7f707a37f63b5b8f37f43b5b8ff6bef6335b97f43b5b97bc371a1313105f28100d131b757f
```


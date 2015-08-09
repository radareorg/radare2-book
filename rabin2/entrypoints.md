## Code Entrypoints

The `-e` option passed to rabin2 will show entrypoints for given binary. Two examples:

    $ rabin2 -e /bin/ls
    [Entrypoints]
    addr=0x00004888 off=0x00004888 baddr=0x00000000

    1 entrypoints

    $ rabin2 -er /bin/ls
    fs symbols
    f entry0 @ 0x00004888
    s entry0


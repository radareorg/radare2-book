## List Libraries

Rabin2 can list libraries used by a binary with the `-l` option:

    $ rabin2 -l /bin/ls
    [Linked libraries]
    libselinux.so.1
    librt.so.1
    libacl.so.1
    libc.so.6
    
    4 libraries


If you compare the outputs of `rabin2 -l` and `ldd`, you will notice that rabin2 lists fewer libraries than `ldd`. The reason is that rabin2 does not follow and does not show dependencies of libraries. Only direct binary dependencies are shown.

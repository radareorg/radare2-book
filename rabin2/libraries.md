## Libraries

Rabin2 can list the libraries used by a binary with the flag -l.

    $ rabin2 -l /bin/ls
    [Linked libraries]
    libselinux.so.1
    librt.so.1
    libacl.so.1
    libc.so.6
    
    4 libraries

    
If you compare the output of 'rabin2 -l' and 'ldd' you will notice that rabin will list less libraries than 'ldd'. The reason is that rabin will not follow the dependencies of the listed libraries, it will just display the ones listed in the binary itself.

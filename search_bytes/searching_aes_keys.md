## 5.7 Searching AES keys
 
Thanks to Victor Mu<here be dragonz>oz i have added support to the algorithm he developed to find expanded AES keys. It runs the search from the current seek to the cfg.limit or the end of the file. You can always stop the search pressing `^C`.

    $ sudo r2 /dev/mem
    [0x00000000]> /A
    0 AES keys found
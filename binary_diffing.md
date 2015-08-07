# Binary Diffing

This section is based on http://radare.today article "[binary diffing](http://radare.today/binary-diffing/)"

Without any parameters, `radiff2` by default shows what bytes are changed and their corresponding offsets:

    $ radiff2 genuine cracked
    0x000081e0 85c00f94c0 => 9090909090 0x000081e0  
    0x0007c805 85c00f84c0 => 9090909090 0x0007c805

    $ rasm2 -d 85c00f94c0
    test eax, eax
    sete al

Notice how the two jumps are nop'ed.

For bulk processing, you may want to have a higher-level overview of differences. This is why radare2 is able to compute the distance and the percentage of similarity between two files with the `-s` option:

    $ radiff2 -s /bin/true /bin/false
    similarity: 0.97
    distance: 743
    
    
If you want more concrete data, it's also possible to count the differences, with the `-c` option:

    $ radiff2 -c genuine cracked
    2  
    
    
If you are unsure whether you are dealing with similar binaries, with `-C` flag you can check there are matching functions. It this mode, it will give you three columns for all functions: "First file offset", "Percentage of matching" and "Second file offset".


    $ radiff2 -C /bin/false /bin/true 
             entry0  0x4013e8 |   MATCH  (0.904762) | 0x4013e2  entry0
             sym.imp.__libc_start_main  0x401190 |   MATCH  (1.000000) | 0x401190  sym.imp.__libc_start_main  
             fcn.00401196  0x401196 |   MATCH  (1.000000) | 0x401196  fcn.00401196
             fcn.0040103c  0x40103c |   MATCH  (1.000000) | 0x40103c  fcn.0040103c
             fcn.00401046  0x401046 |   MATCH  (1.000000) | 0x401046  fcn.00401046
             [...]


And now a cool feature : radare2 supports graph-diffing, à la [DarunGrim](http://www.darungrim.org/), with the `-g` option. You can either give it a symbol name, of specify two offsets, if the function you want to diff is named differently in compared files. For example, `radiff2 -g main /bin/true /bin/false | xdot -` will show differences in `main()` function of Unix `true` and `false` programs. You can compare it to `radiff2 -g main /bin/false /bin/true` (Notice the order of the arguments) to get the two versions.
This is the result:

![/bin/true vs /bin/false](true_false.png)


Parts in yellow indicate that some offsets do not match. The grey piece means a perfect match. The red one highlights a strong difference. If you look closely, you will see that the left part of the picture has `mov edi, 0x1; call sym.imp.exit`, while the right one has `xor edi, edi; call sym.imp.exit`.

Binary diffing is an important feature for reverse engineering. It can be used to analyze [security updates](https://en.wikipedia.org/wiki/Patch_Tuesday), infected binaries, firmware changes and more...

We have only shown the code analysis diffing functionality, but radare2 supports additional types of diffing between two binaries: at byte level, deltified similarities, and more to come.

We have plans to implement more kinds of bindiffing algorithms into r2, and why not, add support for ascii art graph diffing and better integration with the rest of the toolkit.


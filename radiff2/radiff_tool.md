# Binary Diffing

This chapter is based on the radare.today article "[binary diffing](http://radare.today/binary-diffing/)"

Binary diffing is an important part of reverse engineering process. It can be used to analyze [security updates](https://en.wikipedia.org/wiki/Patch_Tuesday), infected binaries, firmware changes, and many more.

`radiff2` takes names of two files to be compared. Without any options, `radiff2` will show what bytes have changed at which offsets. For example:

    $ radiff2 genuine cracked 
    0x00000504 85c07507 => 90909090 0x00000504

    $ rasm2 -d 85c07507
    test eax, eax
    jne 0xb

Notice how test and conditional jump instructions have been nop'ed out.

For bulk file processing, you may want to have a higher-level overview of differences. For this, `radare2` is able to compute distance and similarity percentage of two files. Use `-s` option to see them:

    $ radiff2 -s /bin/true /bin/false
    similarity: 0.97
    distance: 743
    
With `-c` option it is possible to count differences:

    $ radiff2 -c genuine cracked
    2  
    
To make sure that you are working with similar, not completely unrelated, binaries, you can check if there are any matching functions in them. To see such comparison, use `-C` option. In the resulting output, three columns are: the first file function with its offset, percentage of match, and the second file function with its offset. For example:

    $ radiff2 -C /bin/false /bin/true 
             entry0  0x4013e8 |   MATCH  (0.904762) | 0x4013e2  entry0
             sym.imp.__libc_start_main  0x401190 |   MATCH  (1.000000) | 0x401190  sym.imp.__libc_start_main  
             fcn.00401196  0x401196 |   MATCH  (1.000000) | 0x401196  fcn.00401196
             fcn.0040103c  0x40103c |   MATCH  (1.000000) | 0x40103c  fcn.0040103c
             fcn.00401046  0x401046 |   MATCH  (1.000000) | 0x401046  fcn.00401046
             [...]


And here comes the cool feature : radiff2 supports graphical diffing, à la [DarunGrim](http://www.darungrim.org/). You can see it using `-g` option. This option requires either a symbol name as a parameter, or two offsets for corresponding files. The second variant is useful if a function you want to diff does not have the same name in both files.
The output is in [Graphviz](http://graphviz.org/) graph description language. You can convert it to a picture with `dot` program from the Graphviz package.
For example, `radiff2 -g main /bin/true /bin/false | xdot -` will show differences between `main()` functions of `true` and `false` programs. You can also compare the picture against `radiff2 -g main /bin/false /bin/true |xdot -` result (notice a changed order of radiff2 options).

This is the result:

![/bin/true vs /bin/false](true_false.png)

Yellow color indicates that some offsets inside of corresponding blocks being compared do not match. Grey-colored block is a perfect match. Red block highlights a huge difference of two compared blocks. If you look closely, you will see that the left red block has `mov edi, 0x1; call sym.imp.exit`, while the right one has `xor edi, edi; call sym.imp.exit`. Obviously that is because `true` always returns zero, and `false` always returns one.

For now, we have only seen simple code analysis diffing functionality of radiff2. It supports more kinds of diffing between two binaries: at byte level, deltified similarities, and more to come.
There are plans to implement more types of bindiffing functionality into r2, and (why not?) to support ASCII-art graphic output. Also, a better integration with the rest of the toolkit is a nice thing to have.


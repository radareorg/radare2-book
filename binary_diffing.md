# Binary Diffing 

Based on radare.today article "[binary diffing](http://radare.today/binary-diffing/)"

Without parameter, `radiff2` will by default show what bytes changed, and the corresponding offsets.

    $ radiff2 genuine cracked
    0x000081e0 85c00f94c0 => 9090909090 0x000081e0  
    0x0007c805 85c00f84c0 => 9090909090 0x0007c805

    $ rasm2 -d 85c00f94c0
    test eax, eax  
    sete al
    
Notice how the two jumps are noped.

For bulk processing, you may want to have a higher-overview of the differences. This is why radare2 is able to compute the distance and the percentage of similarity between two files with the `-s` option:

    $ radiff2 -s /bin/true /bin/false
    similarity: 0.97  
    distance: 743  
    
    
If you want more concrete data, it's also possible to count the differences, with the `-c` option:

    $ radiff2 -c genuine cracked
    2  
    
    
If you're unsure about the fact that you're dealing with similar binaries, you can check if some functions are matching with the `-C` option. The columns being: "First file offset", "Percentage of matching" and "Second file offset".





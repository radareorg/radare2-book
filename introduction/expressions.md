## Expressions

Expressions are mathematical representations of 64-bit numerical values. They can be displayed in different formats, be compared or used with all commands accepting numeric arguments. Expressions can use traditional arithmetic operations, as well as binary and boolean ones. To evaluate mathematical expressions prepend them with command `?`. For example:

    [0xB7F9D810]> ? 0x8048000
    134512640 0x8048000 01001100000 128.0M 804000:0000 134512640 00000000 134512640.0 0.000000  
    [0xB7F9D810]> ? 0x8048000+34
    134512674 0x8048022 01001100042 128.0M 804000:0022 134512674 00100010 134512674.0 0.000000  
    [0xB7F9D810]> ? 0x8048000+0x34
    134512692 0x8048034 01001100064 128.0M 804000:0034 134512692 00110100 134512692.0 0.000000  
    [0xB7F9D810]> ? 1+2+3-4*3
    -6 0xfffffffffffffffa 01777777777777777777772 17179869183.0G fffff000:0ffa -6   

Supported arithmetic operations are:

 *   \+ : addition
 *   \- : subtraction
 *   \* : multiplication
 *   / : division
 *   % : modulus
 *   \> : shift right
 *   < : shift left

Use of logical OR should be escaped using quotes, or it will be mistaken for a pipe opeator:

    [0x00000000]> "? 1 | 2"
    3 0x3 03 3 0000:0003 3 "\x03" 00000011 2.0 2.000000f 2.000000

Numbers can be displayed in several formats:

    0x033   : hexadecimal can be displayed
    3334    : decimal
    sym.fo  : resolve flag offset
    10K     : KBytes  10*1024
    10M     : MBytes  10*1024*1024

You can also use variables and seek positions to build complex expressions. Available values include:

    ?@?    or type @@?      ; misc help for '@' (seek), '~' (grep) (see ~??)
    ?$?           ; show available '$' variables
    $$            ; here (the current virtual seek)
    $l            ; opcode length
    $s            ; file size
    $j            ; jump address (e.g. jmp 0x10, jz 0x10 => 0x10)
    $f            ; jump fail address (e.g. jz 0x10 => next instruction)
    $m            ; opcode memory reference (e.g. mov eax,[0x10] => 0x10)

Some more examples:

    [0x4A13B8C0]> ? $m + $l
    140293837812900 0x7f98b45df4a4 03771426427372244 130658.0G 8b45d000:04a4 140293837812900 10100100 140293837812900.0 -0.000000


    [0x4A13B8C0]> pd 1 @ +$l
    0x4A13B8C2   call 0x4a13c000

# Visual Disassembly

## Navigation

Move within the Disassembly using `arrows` or `hjkl`. Use `o` to seek directly to a flag or an offset, type it when requested by the prompt: `[offset]>`.
Follow a jump or a call using the `number` of your keyboard `[0-9]` and the number on the right in disassembly to follow a call or a jump. In this example typing `1` on the keyboard would follow the call to `sym.imp.__libc_start_main` and therefore, seek at the offset of this symbol.

```
            0x00404894      e857dcffff     call sym.imp.__libc_start_main ;[1]
```
            
Seek back to the previous location using `u`, `U` will allow you to redo the seek.

## `d` as define

`d` can be used to change the type of data of the current block, several basic types/structures are available as well as more advanced one using `pf` template:

```
d → ...
0x004048f7      48c1e83f       shr rax, 0x3f                                                 
d → b
0x004048f7 .byte 0x48     
d → B
0x004048f7 .word 0xc148                                                                      
d → d
0x004048f7 hex length=165 delta=0                                                            
0x004048f7  48c1 e83f 4801 c648 d1fe 7415 b800 0000  H..?H..H..t.....                                    
...
```

To improve code readability change how radare2 presents numerical values in disassembly, 
By default most of disassembly display numerical value as hexadecimal. Sometimes you would like to view it as a decimal, binary or even custom defined constant. To change value format you can use `d` following by `i` then choose what base to work in, this is the equivalent to `ahi`:

```
d → i → ...
0x004048f7      48c1e83f       shr rax, 0x3f                                                 
d → i →  10
0x004048f7      48c1e83f       shr rax, 63                                                   
d → i →  2
0x004048f7      48c1e83f       shr rax, '?'
```

## Tweaking the Disassembly

You can change the disassembly look and feel by using the [Visual Configuration Editor](visual_configuration_editor.md).

Following are some example of eval variable related to disassembly.

#### asm.arch: Change Architecture && asm.bits: Word size in bits at assembler

You can view the list of all arch using `e asm.arch=?`

```
e asm.arch = dalvik
            0x00404870      31ed4989       cmp-long v237, v73, v137                         
            0x00404874      d15e4889       rsub-int v14, v5, 0x8948
            0x00404878      e24883e4       ushr-int/lit8 v72, v131, 0xe4
            0x0040487c      f0505449c7c0   +invoke-object-init-range {}, method+18772 ;[0]
            0x00404882      90244100       add-int v36, v65, v0
```

```
e asm.bits = 16                                   
            0000:4870      31ed           xor bp, bp                                  
            0000:4872      49             dec cx
            0000:4873      89d1           mov cx, dx 
            0000:4875      5e             pop si
            0000:4876      48             dec ax 
            0000:4877      89e2           mov dx, sp
```



#### asm.pseudo: Enable pseudo syntax

```
e asm.pseudo = true                                   
            0x00404870      31ed           ebp = 0                              
            0x00404872      4989d1         r9 = rdx
            0x00404875      5e             pop rsi
            0x00404876      4889e2         rdx = rsp
            0x00404879      4883e4f0       rsp &= 0xfffffffffffffff0
```

#### asm.syntax: Select assembly syntax (intel, att, masm...)

```
e asm.syntax = att
            0x00404870      31ed           xor %ebp, %ebp
            0x00404872      4989d1         mov %rdx, %r9 
            0x00404875      5e             pop %rsi      
            0x00404876      4889e2         mov %rsp, %rdx
            0x00404879      4883e4f0       and $0xfffffffffffffff0, %rsp
```

#### asm.describe: Show opcode description

```
e asm.describe = true
            0x00404870      31ed           xor ebp, ebp                ; logical exclusive or
            0x00404872      4989d1         mov r9, rdx                 ; moves data from src to dst
            0x00404875      5e             pop rsi                     ; pops last element of stack and stores the result in argument
            0x00404876      4889e2         mov rdx, rsp                ; moves data from src to dst        
            0x00404879      4883e4f0       and rsp, 0xfffffffffffffff0 ; binary and operation between src and dst, stores result on dst
```



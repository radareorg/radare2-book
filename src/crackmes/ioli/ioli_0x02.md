IOLI 0x02
=========

This is the third one.

```
$ ./crackme0x02
IOLI Crackme Level 0x02
Password: hello
Invalid Password!
```

check it with rabin2.
```
$ rabin2 -z ./crackme0x02
[Strings]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000548 0x08048548 24  25   .rodata ascii IOLI Crackme Level 0x02\n
1   0x00000561 0x08048561 10  11   .rodata ascii Password:
2   0x0000056f 0x0804856f 15  16   .rodata ascii Password OK :)\n
3   0x0000057f 0x0804857f 18  19   .rodata ascii Invalid Password!\n
```

similar to 0x01, no explicity password string here. so it's time to analyze it with r2.
```
[0x08048330]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x08048330]> pdf@main
            ; DATA XREF from entry0 @ 0x8048347
/ 144: int main (int argc, char **argv, char **envp);
|           ; var int32_t var_ch @ ebp-0xc
|           ; var int32_t var_8h @ ebp-0x8
|           ; var int32_t var_4h @ ebp-0x4
|           ; var int32_t var_sp_4h @ esp+0x4
|           0x080483e4      55             push ebp
|           0x080483e5      89e5           mov ebp, esp
|           0x080483e7      83ec18         sub esp, 0x18
|           0x080483ea      83e4f0         and esp, 0xfffffff0
|           0x080483ed      b800000000     mov eax, 0
|           0x080483f2      83c00f         add eax, 0xf                ; 15
|           0x080483f5      83c00f         add eax, 0xf                ; 15
|           0x080483f8      c1e804         shr eax, 4
|           0x080483fb      c1e004         shl eax, 4
|           0x080483fe      29c4           sub esp, eax
|           0x08048400      c70424488504.  mov dword [esp], str.IOLI_Crackme_Level_0x02 ; [0x8048548:4]=0x494c4f49 ; "IOLI Crackme Level 0x02\n"
|           0x08048407      e810ffffff     call sym.imp.printf         ; int printf(const char *format)
|           0x0804840c      c70424618504.  mov dword [esp], str.Password: ; [0x8048561:4]=0x73736150 ; "Password: "
|           0x08048413      e804ffffff     call sym.imp.printf         ; int printf(const char *format)
|           0x08048418      8d45fc         lea eax, [var_4h]
|           0x0804841b      89442404       mov dword [var_sp_4h], eax
|           0x0804841f      c704246c8504.  mov dword [esp], 0x804856c  ; [0x804856c:4]=0x50006425
|           0x08048426      e8e1feffff     call sym.imp.scanf          ; int scanf(const char *format)
|           0x0804842b      c745f85a0000.  mov dword [var_8h], 0x5a    ; 'Z' ; 90
|           0x08048432      c745f4ec0100.  mov dword [var_ch], 0x1ec   ; 492
|           0x08048439      8b55f4         mov edx, dword [var_ch]
|           0x0804843c      8d45f8         lea eax, [var_8h]
|           0x0804843f      0110           add dword [eax], edx
|           0x08048441      8b45f8         mov eax, dword [var_8h]
|           0x08048444      0faf45f8       imul eax, dword [var_8h]
|           0x08048448      8945f4         mov dword [var_ch], eax
|           0x0804844b      8b45fc         mov eax, dword [var_4h]
|           0x0804844e      3b45f4         cmp eax, dword [var_ch]
|       ,=< 0x08048451      750e           jne 0x8048461
|       |   0x08048453      c704246f8504.  mov dword [esp], str.Password_OK_: ; [0x804856f:4]=0x73736150 ; "Password OK :)\n"
|       |   0x0804845a      e8bdfeffff     call sym.imp.printf         ; int printf(const char *format)
|      ,==< 0x0804845f      eb0c           jmp 0x804846d
|      |`-> 0x08048461      c704247f8504.  mov dword [esp], str.Invalid_Password ; [0x804857f:4]=0x61766e49 ; "Invalid Password!\n"
|      |    0x08048468      e8affeffff     call sym.imp.printf         ; int printf(const char *format)
|      |    ; CODE XREF from main @ 0x804845f
|      `--> 0x0804846d      b800000000     mov eax, 0
|           0x08048472      c9             leave
\           0x08048473      c3             ret

```

with the experience of solving crackme0x02, we first locate the position of `cmp` instruction by using this simple oneliner:
```
[0x08048330]> pdf@main | grep cmp
|           0x0804844e      3b45f4         cmp eax, dword [var_ch]
```

Unfortunately, the variable compared to eax is stored in the stack. we can't check the value of this variable directly. It's a common case in reverse engineerning that we have to derive the value of the variable from the previous sequence. As the amount of code is relatively small, it's possible.

for example:
```
|           0x080483ed      b800000000     mov eax, 0
|           0x080483f2      83c00f         add eax, 0xf                ; 15
|           0x080483f5      83c00f         add eax, 0xf                ; 15
|           0x080483f8      c1e804         shr eax, 4
|           0x080483fb      c1e004         shl eax, 4
|           0x080483fe      29c4           sub esp, eax
```

we can easily get the value of eax. it's 0x16.

It gets hard when the scale of program grows. radare2 provides a pseudo disassembler output in C-like syntax. It may be useful.
```
[0x08048330]> pdc@main
function main () {
    //  4 basic blocks

    loc_0x80483e4:

         //DATA XREF from entry0 @ 0x8048347
       push ebp
       ebp = esp
       esp -= 0x18
       esp &= 0xfffffff0
       eax = 0
       eax += 0xf               //15
       eax += 0xf               //15
       eax >>>= 4
       eax <<<= 4
       esp -= eax
       dword [esp] = "IOLI Crackme Level 0x02\n" //[0x8048548:4]=0x494c4f49 ; str.IOLI_Crackme_Level_0x02 ; const char *format
                                                   
       int printf("IOLI Crackme Level 0x02\n")
       dword [esp] = "Password: " //[0x8048561:4]=0x73736150 ; str.Password: ; const char *format
                                                   
       int printf("Password: ")
       eax = var_4h
       dword [var_sp_4h] = eax
       dword [esp] = 0x804856c  //[0x804856c:4]=0x50006425 ; const char *format
       int scanf("%d") 
                               //sym.imp.scanf ()
       dword [var_8h] = 0x5a    //'Z' ; 90
       dword [var_ch] = 0x1ec   //492
       edx = dword [var_ch]
       eax = var_8h             //"Z"
       dword [eax] += edx
       eax = dword [var_8h]
       eax = eax * dword [var_8h]
       dword [var_ch] = eax
       eax = dword [var_4h]
       var = eax - dword [var_ch]
       if (var) goto 0x8048461  //likely
       {
        loc_0x8048461:

           //CODE XREF from main @ 0x8048451
           dword [esp] = s"Invalid Password!\n"//[0x804857f:4]=0x61766e49 ; str.Invalid_Password ; const char *format
                                                       
           int printf("Invalid ")
       do
       {
            loc_0x804846d:

               //CODE XREF from main @ 0x804845f
               eax = 0
               leave                    //(pstr 0x0804857f) "Invalid Password!\n" ebp ; str.Invalid_Password
               return
           } while (?);
       } while (?);
      }
      return;

}
```

The `pdc` command is unreliable especially in processing loops (while, for, etc.). So I prefer to use the [r2dec](https://github.com/radareorg/r2dec-js) plugin in r2 repo to generate the pseudo C code. you can install it easily:
```
r2pm install r2dec
```

decompile `main()` with the following command (like `F5` in IDA):
```C
[0x08048330]> pdd@main
/* r2dec pseudo code output */
/* ./crackme0x02 @ 0x80483e4 */
#include <stdint.h>

int32_t main (void) {
    uint32_t var_ch;
    int32_t var_8h;
    int32_t var_4h;
    int32_t var_sp_4h;
    eax = 0;
    eax += 0xf;
    eax += 0xf;
    eax >>= 4;
    eax <<= 4;
    printf ("IOLI Crackme Level 0x02\n");
    printf ("Password: ");
    eax = &var_4h;
    *((esp + 4)) = eax;
    scanf (0x804856c);
    var_8h = 0x5a;
    var_ch = 0x1ec;
    edx = 0x1ec;
    eax = &var_8h;
    *(eax) += edx;
    eax = var_8h;
    eax *= var_8h;
    var_ch = eax;
    eax = var_4h;
    if (eax == var_ch) {
        printf ("Password OK :)\n");
    } else {
        printf ("Invalid Password!\n");
    }
    eax = 0;
    return eax;
}
```

It's more human-readable now. To check the string in 0x804856c,
we can:
* seek
* print string
```
[0x08048330]> s 0x804856c
[0x0804856c]> ps
%d
```
it's exactly the format string of `scanf()`. But r2dec does not recognize the second argument (eax) which is a pointer. it points to var_4h and means our input will store in var_4h.

we can easily write out pseudo code here.
```C
var_ch = (var_8h + var_ch)^2;
if (var_ch == our_input)
  printf("Password OK :)\n");
```

given the initial status that var_8h is 0x5a, var_ch is 0x1ec, we have 
var_ch = 338724 (0x52b24):

```
$ rax2 '=10' '(0x5a+0x1ec)*(0x5a+0x1ec)' 
338724

$ ./crackme0x02
IOLI Crackme Level 0x02
Password: 338724
Password OK :)
```

and we finish the crackme0x02.

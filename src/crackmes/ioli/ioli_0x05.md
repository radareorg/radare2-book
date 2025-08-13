### IOLI 0x05

check again, it uses `scanf()` to get our input and pass it to `check()` as parameter.

```c
[0x080483d0]> pdd@main
/* r2dec pseudo code output */
/* ./crackme0x05 @ 0x8048540 */
#include <stdint.h>

int32_t main (void) {
    int32_t var_78h;
    int32_t var_4h;
    eax = 0;
    eax += 0xf;
    eax += 0xf;
    eax >>= 4;
    eax <<= 4;
    printf ("IOLI Crackme Level 0x05\n");
    printf ("Password: ");
    eax = &var_78h;
    scanf (0x80486b2, eax);			// 0x80486b2 is %s
    eax = &var_78h;
    check (eax);
    eax = 0;
    return eax;
}
```

the check() function:

```c
/* r2dec pseudo code output */
/* ./crackme0x05 @ 0x80484c8 */
#include <stdint.h>

int32_t check (char * s) {
    char * var_dh;
    uint32_t var_ch;
    uint32_t var_8h;
    int32_t var_4h;
    char * format;
    int32_t var_sp_8h;
    var_8h = 0;
    var_ch = 0;
    do {
        eax = s;
        eax = strlen (eax);
        if (var_ch >= eax) {
            goto label_0;
        }
        eax = var_ch;
        eax += s;
        eax = *(eax);
        var_dh = al;
        eax = &var_4h;
        eax = &var_dh;
        sscanf (eax, eax, 0x8048668);			// 0x8048668 is %d
        edx = var_4h;
        eax = &var_8h;
        *(eax) += edx;
        if (var_8h == 0x10) {
            eax = s;
            parell (eax);
        }
        eax = &var_ch;
        *(eax)++;
    } while (1);
label_0:
    printf ("Password Incorrect!\n");
    return eax;
}
```

The same, we can write our own C-like pseudo code.

```c
#include <stdint.h>
int32_t check(char *s)
{
    var_ch = 0;
    var_8h = 0;
    for (var_ch = 0; var_ch < strlen(s); ++var_ch)
    {
        var_dh = s[var_ch];
        sscanf(&var_dh, %d, &var_4h);			// read from string[var_ch], store to var_4h
        var_8h += var_4h;
        if(var_8h == 0x10)
            parell(s);
    }
    printf("Password Incorrect!\n");
    return 0;
}
```

The if condition becomes `var_8h == 0x10`. In addition, a new function call - `parell(s)` replace the `printf("password OK")`now. The next step is to reverse sym.parell.

```c
[0x08048484]> s sym.parell
[0x08048484]> pdd@sym.parell
/* r2dec pseudo code output */
/* ./crackme0x05 @ 0x8048484 */
#include <stdint.h>

uint32_t parell (char * s) {
    int32_t var_4h;
    char * format;
    int32_t var_8h;
    eax = &var_4h;
    eax = s;
    sscanf (eax, eax, 0x8048668);
    eax = var_4h;
    eax &= 1;
    if (eax == 0) {
        printf ("Password OK!\n");
        exit (0);
    }
    return eax;
}
```

the decompiled code looks well except the `sscanf()` function. It can be easily corrected by checking the assembly code.

```asm
/ 68: sym.parell (int32_t arg_8h);
|           ; var int32_t var_4h @ ebp-0x4
|           ; arg int32_t arg_8h @ ebp+0x8
|           ; var int32_t var_sp_4h @ esp+0x4
|           ; var int32_t var_8h @ esp+0x8
|           0x08048484      55             push ebp
|           0x08048485      89e5           mov ebp, esp
|           0x08048487      83ec18         sub esp, 0x18
|           0x0804848a      8d45fc         lea eax, [var_4h]
|           0x0804848d      89442408       mov dword [var_8h], eax
|           0x08048491      c74424046886.  mov dword [var_sp_4h], 0x8048668 ; [0x8048668:4]=0x50006425 %d
|           0x08048499      8b4508         mov eax, dword [arg_8h]
|           0x0804849c      890424         mov dword [esp], eax
|           0x0804849f      e800ffffff     call sym.imp.sscanf         ; int sscanf(const char *s, const char *format,   ...)
....
```

The `mov dword [esp], eax` is the nearest instruction to sscanf (and it's equivalent to a push instruction). It stores the string 's' to the stack top (arg1).  `mov dword [var_sp_4h], 0x8048668` push '%d' as arg2 into stack. var_8h (esp + 0x8) which keeps the address of var_4h is the arg3.

Finally we have the corrected pseudo code:

```c
uint32_t parell (char * s) {
    sscanf (s, %d, &var_4h);
    if ((var_4h & 1) == 0) {
        printf ("Password OK!\n");
        exit(0);
    }
    return 0;
}
```

Now there are 2 constraints:

* Digit Sum is 16 (0x10)
* Must be an even number (1 & number == 0)

The password is at our fingertips now.

```console
$ ./crackme0x05
IOLI Crackme Level 0x05
Password: 88
Password OK!

$ ./crackme0x05
IOLI Crackme Level 0x05
Password: 12346
Password OK!
```

we can also use angr to solve it since we have two constraints to the password.

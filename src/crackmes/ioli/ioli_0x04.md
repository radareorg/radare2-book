### IOLI 0x04

```C
[0x080483d0]> pdd@main
/* r2dec pseudo code output */
/* ./crackme0x04 @ 0x8048509 */
#include <stdint.h>

int32_t main (void) {
    int32_t var_78h;
    int32_t var_4h;
    eax = 0;
    eax += 0xf;
    eax += 0xf;
    eax >>= 4;
    eax <<= 4;
    printf ("IOLI Crackme Level 0x04\n");
    printf ("Password: ");
    eax = &var_78h;
    scanf (0x8048682, eax);
    eax = &var_78h;
    check (eax);
    eax = 0;
    return eax;
}
```

Let's enter check.

```C
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
        sscanf (eax, eax, 0x8048638);
        edx = var_4h;
        eax = &var_8h;
        *(eax) += edx;
        if (var_8h == 0xf) {
            printf ("Password OK!\n");
            exit (0);
        }
        eax = &var_ch;
        *(eax)++;
    } while (1);
label_0:
    printf ("Password Incorrect!\n");
    return eax;
}
```

manually analyze with both the assembly and pseudo code we can simply write down the C-like code to describe this function:

```C
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
        if(var_8h == 0xf)
            printf("Password OK\n");
    }
    printf("Password Incorrect!\n");
    return 0;
}
```

In short, it calculates the Digit Sum of a number (add a number digit by digit. for example, 96 => 9 + 6 = 15) :

```sh
./crackme0x04
IOLI Crackme Level 0x04
Password: 12345
Password OK!

./crackme0x04
IOLI Crackme Level 0x04
Password: 96
Password OK!
```


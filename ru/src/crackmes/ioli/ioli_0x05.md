IOLI 0x05
=========

смотрим, использется `scanf()` для получения ввода, затем он передается в функцию `check()` в качестве параметра.

```C
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

```C
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

Точно так же можно написать наш собственный C-подобный псевдокод.

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
        if(var_8h == 0x10)
            parell(s);
    }
    printf("Password Incorrect!\n");
    return 0;
}
```

Условие в if - `var_8h == 0x10`. Кроме того, новый вызов функции - `parell(s)` заменяет тепрь `printf("пароль OK")`. Следующим шагом является взлом sym.parell.

```C
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

с декомпилированным кодом все в порядке, за исключением функции `sscanf(`). Его можно легко исправить, посмотрев ассемблерный код.

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

Инструкция `mov dword [esp], eax` является ближайшей к sscanf (она эквивалентна инструкции push). Он сохраняет строку 's' на вершине стека (arg1).  `mov dword [var_sp_4h], 0x8048668` засылает '%d' как arg2 в стек. var_8h (esp + 0x8) хранит адрес var_4h, т.е. arg3.

Наконец, исправленный псевдокод -

```C
uint32_t parell (char * s) {
    sscanf (s, %d, &var_4h);
    if ((var_4h & 1) == 0) {
        printf ("Password OK!\n");
        exit(0);
    }
    return 0;
}
```

Выявлены два ограничения:

* Сумма цифр должна быть равна 16 (0x10)
* Должно быть нечетным числом (1 & number == 0)

Пароль теперь у нас под рукой.

```sh
./crackme0x05
IOLI Crackme Level 0x05
Password: 88
Password OK!

./crackme0x05
IOLI Crackme Level 0x05
Password: 12346
Password OK!
```

также можно использовать angr для решения этой задачи, так как заданы два ограничения на пароль.

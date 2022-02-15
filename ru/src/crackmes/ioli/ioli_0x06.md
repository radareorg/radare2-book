IOLI 0x06
=========

долгий процесс анализа этого двоичного файла (не полное представление результата далее в тексте):

```shell
rabin2 -z ./crackme0x06
[Strings]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000738 0x08048738 4   5    .rodata ascii LOLO
1   0x00000740 0x08048740 13  14   .rodata ascii Password OK!\n
2   0x0000074e 0x0804874e 20  21   .rodata ascii Password Incorrect!\n
3   0x00000763 0x08048763 24  25   .rodata ascii IOLI Crackme Level 0x06\n
4   0x0000077c 0x0804877c 10  11   .rodata ascii Password:

rabin2 -I ./crackme0x06
arch     x86
baddr    0x8048000
bintype  elf
bits     32
compiler GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
crypto   false
endian   little
havecode true
lang     c
machine  Intel 80386
maxopsz  16
minopsz  1
os       linux
static   false
va       true
```

проанализировав ответ, декомпилируем main

```C
[0x08048400]> pdd@main
/* r2dec pseudo code output */
/* ./crackme0x06 @ 0x8048607 */
#include <stdint.h>

int32_t main (int32_t arg_10h) {
    int32_t var_78h;
    int32_t var_4h;
    // выравнивание стека
    eax = 0;
    eax += 0xf;
    eax += 0xf;
    eax >>= 4;
    eax <<= 4;

    // основная логика
    printf ("IOLI Crackme Level 0x06\n");
    printf ("Password: ");
    eax = &var_78h;
    scanf (0x8048787, eax);
    eax = arg_10h;
    eax = &var_78h;
    check (eax, arg_10h);
    eax = 0;
    return eax;
}
```

main-у передаются три аргумента `argc, argv, envp`, и эта программа компилировалась GCC, поэтому стек должен быть таким:

```sh
[esp + 0x10] - envp
[esp + 0x0c] - argv
[esp + 0x08] - argc
[esp + 0x04] - return address
```

входим в функцию `check()` и декомпилируем ее. функция отличается от той, что была в задаче 0x05. но они по-прежнему имеют схожие структуры кода.

```C
int32_t check (char * s, int32_t arg_ch) {
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
        sscanf (eax, eax, 0x804873d);
        edx = var_4h;
        eax = &var_8h;
        *(eax) += edx;
        if (var_8h == 0x10) {
            eax = arg_ch;
            eax = s;
            parell (eax, arg_ch);
        }
        eax = &var_ch;
        *(eax)++;
    } while (1);
label_0:
    printf ("Password Incorrect!\n");
    return eax;
}
```

Надо исправить `sscanf` и `parell`, обе они были сгенерированы неправильно:

```C
int32_t check (char * s, void* envp)
{
    var_ch = 0;
    var_8h = 0;
    for (var_ch = 0; var_ch < strlen(s); ++var_ch)
    {
        var_dh = s[var_ch];
        sscanf(&var_dh, %d, &var_4h);			// read from string[var_ch], store to var_4h
        var_8h += var_4h;
        if(var_8h == 0x10)
            parell(s, envp);
    }
    printf("Password Incorrect!\n");
    return 0;
}
```

больше никакой информации, надо снова взламывать `parell()`.

```C
#include <stdint.h>

uint32_t parell (char * s, char * arg_ch) {
    sscanf (s, %d, &var_4h);

    if (dummy (var_4h, arg_ch) == 0)
        return 0;

    for (var_bp_8h = 0; var_bp_8h <= 9; ++var_bp_8h){
        if (var_4h & 1 == 0){
            printf("Password OK!\n");
            exit(0);
        }
    }

    return 0;
}
```

появилось новое условие в `parell()` -- `dummy (var_4h, arg_ch) == 0`. тогда взламываем dummy!

```C
[0x080484b4]> pdd@sym.dummy
/* r2dec pseudo code output */
/* ./crackme0x06 @ 0x80484b4 */
#include <stdint.h>

int32_t dummy (char ** s1) {
    int32_t var_8h;
    int32_t var_4h;
    char * s2;
    size_t * n;
    var_4h = 0;
    do {
        eax = 0;
        edx = eax*4;
        eax = s1;
        if (*((edx + eax)) == 0) {
            goto label_0;
        }
        eax = var_4h;
        ecx = eax*4;
        edx = s1;
        eax = &var_4h;
        *(eax)++;
        eax = *((ecx + edx));
        eax = strncmp (eax, 3, "LOLO");
    } while (eax != 0);
    var_8h = 1;
    goto label_1;
label_0:
    var_8h = 0;
label_1:
    eax = 0;
    return eax;
}
```

выглядит как цикл обработки строки. можем "украсить" его.

```C
[0x080484b4]> pdd@sym.dummy
/* r2dec pseudo code output */
/* ./crackme0x06 @ 0x80484b4 */
#include <stdint.h>

int32_t dummy (char ** s1) {
    for (var_4h = 0; strncmp(s1[var_4h], "LOLO", 3) != 0; var_4h++){
        if (s1[i] == NULL)
            return 0;
    }
    return 1;
}
```

Теперь имеем три ограничения в crackme_0x06:

* Сумма цифр
* Нечетное число
* должна быть переменная среды, имя которой начинается с "LOL".

```sh
$ ./crackme0x06
IOLI Crackme Level 0x06
Password: 12346
Password Incorrect!
$ export LOLAA=help
$ ./cracke0x06
IOLI Crackme Level 0x06
Password: 12346
Password OK!
```

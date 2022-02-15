IOLI 0x07
=========

Странная строка "wtf?".

```sh
$ rabin2 -z ./crackme0x07
[Строки]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x000007a8 0x080487a8 4   5    .rodata ascii LOLO
1   0x000007ad 0x080487ad 20  21   .rodata ascii Password Incorrect!\n
2   0x000007c5 0x080487c5 13  14   .rodata ascii Password OK!\n
3   0x000007d3 0x080487d3 5   6    .rodata ascii wtf?\n
4   0x000007d9 0x080487d9 24  25   .rodata ascii IOLI Crackme Level 0x07\n
5   0x000007f2 0x080487f2 10  11   .rodata ascii Password:
```

Опять же, нет строки пароля или сравнений в `main()`. Приведем здесь уже упрощенный псевдокод. Тип переменной var_78h вероятно char *pointer (string).

```asm
#include <stdint.h>
int32_t main (int32_t arg_10h) {
    printf ("IOLI Crackme Level 0x07\n");
    printf ("Password: ");
    scanf (%s, &var_78h);
    return fcn_080485b9 (&var_78h, arg_10h);
}
```

Из-за потери информации ни `aa`,  ни`aaa` не определили названий функций. Можно дважды проверить "flagspace". Radare2 использует fcn_080485b9 в качестве имен функций - распространенный случай в реверс-инжениринге, когда нет никакой информации о символах в двоичном файле.

```sh
[0x080487fd]> fs symbols
[0x080487fd]> f
0x08048400 33 entry0
0x0804867d 92 main
0x080487a4 4 obj._IO_stdin_used
```

Декомпилируем `fcn_080485b9()`:

```C
[0x080485b9]> pdfc
            ; CALL XREF from main @ 0x80486d4
/ 118: fcn.080485b9 (char *s, int32_t arg_ch);
|           ; var char *var_dh @ ebp-0xd
|           ; var signed int var_ch  { >= 0xffffffffffffffff} @ ebp-0xc
|           ; var uint32_t var_8h @ ebp-0x8
|           ; var int32_t var_bp_4h @ ebp-0x4
|           ; arg char *s @ ebp+0x8
|           ; arg int32_t arg_ch @ ebp+0xc
|           ; var char *format @ esp+0x4
|           ; var int32_t var_sp_8h @ esp+0x8
|           0x080485b9      55             push ebp
|           0x080485ba      89e5           mov ebp, esp
|           0x080485bc      83ec28         sub esp, 0x28
|           0x080485bf      c745f8000000.  mov dword [var_8h], 0
|           0x080485c6      c745f4000000.  mov dword [var_ch], 0
|           ; CODE XREF from fcn.080485b9 @ 0x8048628
|       .-> 0x080485cd      8b4508         mov eax, dword [s]
|       :   0x080485d0      890424         mov dword [esp], eax        ; const char *s
|       :   0x080485d3      e8d0fdffff     call sym.imp.strlen         ; size_t strlen(const char *s)
|       :   0x080485d8      3945f4         cmp dword [var_ch], eax
|      ,==< 0x080485db      734d           jae 0x804862a
|      |:   0x080485dd      8b45f4         mov eax, dword [var_ch]
|      |:   0x080485e0      034508         add eax, dword [s]
|      |:   0x080485e3      0fb600         movzx eax, byte [eax]
|      |:   0x080485e6      8845f3         mov byte [var_dh], al
|      |:   0x080485e9      8d45fc         lea eax, [var_bp_4h]
|      |:   0x080485ec      89442408       mov dword [var_sp_8h], eax  ;   ...
|      |:   0x080485f0      c7442404c287.  mov dword [format], 0x80487c2 ; [0x80487c2:4]=0x50006425 ; const char *format
|      |:   ;-- eip:
|      |:   0x080485f8      8d45f3         lea eax, [var_dh]
|      |:   0x080485fb      890424         mov dword [esp], eax        ; const char *s
|      |:   0x080485fe      e8c5fdffff     call sym.imp.sscanf         ; int sscanf(const char *s, const char *format,   ...)
|      |:   0x08048603      8b55fc         mov edx, dword [var_bp_4h]
|      |:   0x08048606      8d45f8         lea eax, [var_8h]
|      |:   0x08048609      0110           add dword [eax], edx
|      |:   0x0804860b      837df810       cmp dword [var_8h], 0x10
|     ,===< 0x0804860f      7512           jne 0x8048623
|     ||:   0x08048611      8b450c         mov eax, dword [arg_ch]
|     ||:   0x08048614      89442404       mov dword [format], eax     ; char *arg_ch
|     ||:   0x08048618      8b4508         mov eax, dword [s]
|     ||:   0x0804861b      890424         mov dword [esp], eax        ; char *s
|     ||:   0x0804861e      e81fffffff     call fcn.08048542
|     ||:   ; CODE XREF from fcn.080485b9 @ 0x804860f
|     `---> 0x08048623      8d45f4         lea eax, [var_ch]
|      |:   0x08048626      ff00           inc dword [eax]
|      |`=< 0x08048628      eba3           jmp 0x80485cd
|      |    ; CODE XREF from fcn.080485b9 @ 0x80485db
\      `--> 0x0804862a      e8f5feffff     call fcn.08048524
```

Уже знакомы со структурой этого кода из предыдущих задач (функция проверки). Отсутствие информации о символах не создает нам больших трудностей. При желании можно использовать команду `afn` для переименования имен функций.

```C
int32_t fcn_080485b9 (char * s, void* envp)
{
    var_ch = 0;
    var_8h = 0;
    for (var_ch = 0; var_ch < strlen(s); ++var_ch)
    {
        var_dh = s[var_ch];
        sscanf(&var_dh, %d, &var_4h);			// read from string[var_ch], store to var_4h
        var_8h += var_4h;
        if(var_8h == 0x10)
            fcn_08048542(s, envp);
    }
    return fcn_08048524();
}
```

Большая часть crackme 0x07 схожа с 0x06. Задачу можно решить с помощью того же пароля и переменных среды:

```sh
$ export LOLAA=help
$ ./cracke0x07
IOLI Crackme Level 0x07
Password: 12346
Password OK!
```

Но ... где находится 'wtf?'. Обычное дело в реверс-инжениринге искать перекрестные ссылки (xref) на строки (данные, функции и т.д.). Соответствующие команды в Radare2 находятся в группе имен "ax":

```sh
[0x08048400]> f
0x080487a8 5 str.LOLO
0x080487ad 21 str.Password_Incorrect
0x080487c5 14 str.Password_OK
0x080487d3 6 str.wtf
0x080487d9 25 str.IOLI_Crackme_Level_0x07
0x080487f2 11 str.Password:
[0x08048400]> axt 0x80487d3
(nofunc) 0x804865c [DATA] mov dword [esp], str.wtf
[0x08048400]> axF str.wtf
Ищем ссылки на флаги, соответствующие 'str.wtf'...
[0x001eff28-0x001f0000] (nofunc) 0x804865c [DATA] mov dword [esp], str.wtf
Macro 'findstref' removed.
```

`[DATA] mov dword [esp], str.wtf` по адресу `0x804865c` - инструкция fcn.080485b9. Но анализ на ПК игнорирует оставшиеся инструкции и отображает только неполный текст дизассемблирования. Диапазон адресов fcn.080485b9 должен быть `0x080485b9 ~ 0x0804867c`. Сбросим размер блока и распечатаем текст вместе с опкодом.

```
[0x08040000]> s 0x080485b9
[0x080485b9]> b 230
[0x08048400]> pd
...
            0x0804862f      8b450c         mov eax, dword [ebp + 0xc]
            0x08048632      89442404       mov dword [esp + 4], eax
            0x08048636      8b45fc         mov eax, dword [ebp - 4]
            0x08048639      890424         mov dword [esp], eax        ; char **s1
            0x0804863c      e873feffff     call fcn.080484b4
            0x08048641      85c0           test eax, eax
        ,=< 0x08048643      7436           je 0x804867b
        |   0x08048645      c745f4000000.  mov dword [ebp - 0xc], 0
        |   ; CODE XREF from fcn.080485b9 @ +0xc0
       .--> 0x0804864c      837df409       cmp dword [ebp - 0xc], 9
      ,===< 0x08048650      7f29           jg 0x804867b
      |:|   0x08048652      8b45fc         mov eax, dword [ebp - 4]
      |:|   0x08048655      83e001         and eax, 1
      |:|   0x08048658      85c0           test eax, eax
     ,====< 0x0804865a      7518           jne 0x8048674
     ||:|   0x0804865c      c70424d38704.  mov dword [esp], str.wtf    ; [0x80487d3:4]=0x3f667477 ; "wtf?\n" ; const char *format
     ||:|   0x08048663      e850fdffff     call sym.imp.printf         ; int printf(const char *format)
     ||:|   0x08048668      c70424000000.  mov dword [esp], 0          ; int status
     ||:|   0x0804866f      e874fdffff     call sym.imp.exit           ; void exit(int status)
     ||:|   ; CODE XREF from fcn.080485b9 @ +0xa1
     `----> 0x08048674      8d45f4         lea eax, [ebp - 0xc]
      |:|   0x08048677      ff00           inc dword [eax]
      |`==< 0x08048679      ebd1           jmp 0x804864c
      | |   ; CODE XREFS from fcn.080485b9 @ +0x8a, +0x97
      `-`-> 0x0804867b      c9             leave
            0x0804867c      c3             ret

```

`test eax, ea;je 0x804867b` перейдет к инструкциям `leave; ret`, что пропускает часть str.wtf кода. Только использование `aa` при анализе этого двоичного файла может отобразить всю функцию.

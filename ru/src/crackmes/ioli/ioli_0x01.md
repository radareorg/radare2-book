IOLI 0x01
=========

Вторая задачка IOLI crackme.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: test
Invalid Password!
```

Посмотрим перечень строк при помощи rabin2.

```
$ rabin2 -z ./crackme0x01
[Strings]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000528 0x08048528 24  25   .rodata ascii IOLI Crackme Level 0x01\n
1   0x00000541 0x08048541 10  11   .rodata ascii Password:
2   0x0000054f 0x0804854f 18  19   .rodata ascii Invalid Password!\n
3   0x00000562 0x08048562 15  16   .rodata ascii Password OK :)\n
```

Видно, что взлом будет не на столько прост как в задаче 0x00. Попробуем дизассемблоровать, используя r2.

```
$ r2 ./crackme0x01
-- Use `zoom.byte=printable` in zoom mode ('z' in Visual mode) to find strings
[0x08048330]> aa
[0x08048330]> pdf@main
            ; DATA XREF from entry0 @ 0x8048347
/ 113: int main (int argc, char **argv, char **envp);
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
|           0x08048400      c70424288504.  mov dword [esp], str.IOLI_Crackme_Level_0x01 ; [0x8048528:4]=0x494c4f49 ; "IOLI Crackme Level 0x01\n"
|           0x08048407      e810ffffff     call sym.imp.printf         ; int printf(const char *format)
|           0x0804840c      c70424418504.  mov dword [esp], str.Password: ; [0x8048541:4]=0x73736150 ; "Password: "
|           0x08048413      e804ffffff     call sym.imp.printf         ; int printf(const char *format)
|           0x08048418      8d45fc         lea eax, [var_4h]
|           0x0804841b      89442404       mov dword [var_sp_4h], eax
|           0x0804841f      c704244c8504.  mov dword [esp], 0x804854c  ; [0x804854c:4]=0x49006425
|           0x08048426      e8e1feffff     call sym.imp.scanf          ; int scanf(const char *format)
|           0x0804842b      817dfc9a1400.  cmp dword [var_4h], 0x149a
|       ,=< 0x08048432      740e           je 0x8048442
|       |   0x08048434      c704244f8504.  mov dword [esp], str.Invalid_Password ; [0x804854f:4]=0x61766e49 ; "Invalid Password!\n"
|       |   0x0804843b      e8dcfeffff     call sym.imp.printf         ; int printf(const char *format)
|      ,==< 0x08048440      eb0c           jmp 0x804844e
|      |`-> 0x08048442      c70424628504.  mov dword [esp], str.Password_OK_: ; [0x8048562:4]=0x73736150 ; "Password OK :)\n"
|      |    0x08048449      e8cefeffff     call sym.imp.printf         ; int printf(const char *format)
|      |    ; CODE XREF from main @ 0x8048440
|      `--> 0x0804844e      b800000000     mov eax, 0
|           0x08048453      c9             leave
\           0x08048454      c3             ret
```

Команда «aa» заставляет r2 проанализировать весь двоичный файл, в результате, среди прочего, получим имена символов (идентификаторов сущностей).

Комбинация "pdf" - это сокращение от

* Print (распечатать)

* Disassemble (в дизассемблированном виде)

* Function (функцию)

Команда распечатает код основной функции в виде ассемберных инструкций, т.е. известной всем функции `main()`. В тексте много всего, включая странные имена, стрелки и т.д.

* "imp." - сокращение от "imports". Это импортированные символы, такие как printf()

* "str." - сокращение от "strings". Это, очевидно, строки.

Смотрим внимательно, находим инструкцию `cmp` с константой 0x149a. `cmp` - инструкция сравнения в архитектуре x86, а 0x - префикс, задающий 16-ричную систему счисления (hexadecimal, hex) для числа константы.

```
0x0804842b    817dfc9a140. cmp dword [ebp + 0xfffffffc], 0x149a
```

Можно использовать команду radare2 `?` для распечатки 0x149a в другой системе счисления.

```
[0x08048330]> ? 0x149a
int32   5274
uint32  5274
hex     0x149a
octal   012232
unit    5.2K
segment 0000:049a
string  "\x9a\x14"
fvalue: 5274.0
float:  0.000000f
double: 0.000000
binary  0b0001010010011010
trits   0t21020100
```

Теперь понятно, что 0x149a равно 5274 в десятичной системе счисления. Давайте попробуем это в качестве пароля.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: 5274
Password OK :)
```

Бинго, пароль - 5274. В этом случае функция пароля в 0x0804842b сравнивала входные данные со значением 0x149a (шестнадцатеричный формат). Поскольку в вводят числа обычно в десятичной системе счисления, нетрудно догадаться, что ввод пароля должен быть в десятичном формате или 5274. Поскольку мы хакеры, и любопытство движет нами, давайте посмотрим, что происходит, когда мы вводим в шестнадцатеричном виде.

```
$ ./crackme0x01
IOLI Crackme Level 0x01
Password: 0x149a
Invalid Password!
```

Стоило попытатья, но не сработало. Проблема в том, что `scanf()` принимает 0 в 0x149a как ноль, а остальные данные в связи с этим не принимаются как шестнадцатеричное значение.

На этом все с задачей IOLI 0x01.

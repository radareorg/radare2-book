IOLI 0x03
=========

Задача crackme 0x03. Пропустим поиск строк и сразу все проанализируем.

```C
[0x08048360]> aaa
[0x08048360]> pdd@sym.main
/* r2dec pseudo code output */
/* ./crackme0x03 @ 0x8048498 */
#include <stdint.h>

int32_t main (void) {
    int32_t var_ch;
    int32_t var_8h;
    int32_t var_4h;
    int32_t var_sp_4h;
    eax = 0;
    eax += 0xf;
    eax += 0xf;
    eax >>= 4;
    eax <<= 4;
    printf ("IOLI Crackme Level 0x03\n");
    printf ("Password: ");
    eax = &var_4h;
    scanf (0x8048634, eax);
    var_8h = 0x5a;
    var_ch = 0x1ec;
    edx = 0x1ec;
    eax = &var_8h;
    *(eax) += edx;
    eax = var_8h;
    eax *= var_8h;
    var_ch = eax;
    eax = var_4h;
    test (eax, eax);
    eax = 0;
    return eax;
}
```

Выглядит просто, за исключением функции `test(eax, eax)`. Необычно вызывать функцию с двумя одинаковыми параметрами, можно предположить, что декомпиляция прошла не совсем так, как требуется. проверим в виде инструкций процессора.

```asm
[0x08048360]> pdf@sym.main
...
           0x080484fc      8945f4         mov dword [var_ch], eax
           0x080484ff      8b45f4         mov eax, dword [var_ch]
           0x08048502      89442404       mov dword [var_sp_4h], eax   ; uint32_t arg_ch
           0x08048506      8b45fc         mov eax, dword [var_4h]
           0x08048509      890424         mov dword [esp], eax         ; int32_t arg_8h
           0x0804850c      e85dffffff     call sym.test
...
```

В наличии `sym.test`, вызываемый с двумя параметрами. Один параметр - var_4h (наш ввод из `scanf()`). Другой - var_ch. Значение var_ch (как параметр `test()`) можно вычислить, как это было в задаче crackme_0x02. Оно равно 0x52b24. Пробуем!

```sh
./crackme0x03
IOLI Crackme Level 0x03
Password: 338724
Password OK!!! :)
```

Смотрим на `sym.test`. Это условный переход с двумя ветвями, сравнивающий два параметрами, потом производится сдвиг. Догадка состоит в том, сдвиг, скорее всего, является частью некоторой процедуры расшифровки (шифр сдвига, например, шифр Цезаря).

```C
/* r2dec pseudo code output */
/* ./crackme0x03 @ 0x804846e */
#include <stdint.h>

int32_t test (int32_t arg_8h, uint32_t arg_ch) {
    eax = arg_8h;
    if (eax != arg_ch) {
        shift ("Lqydolg#Sdvvzrug$");
    } else {
        shift ("Sdvvzrug#RN$$$#=,");
    }
    return eax;
}
```

взломаем `shift()`, удовлетворим наше любопытство.

```asm
[0x08048360]> pdf@sym.shift
        ; CODE (CALL) XREF 0x08048491 (sym.test)
        ; CODE (CALL) XREF 0x08048483 (sym.test)
/ function: sym.shift (90)
|       0x08048414  sym.shift:
|       0x08048414     55               push ebp
|       0x08048415     89e5             mov ebp, esp
|       0x08048417     81ec98000000     sub esp, 0x98
|       0x0804841d     c7458400000000   mov dword [ebp-0x7c], 0x0  ; this seems to be a counter
|  .    ; CODE (JMP) XREF 0x0804844e (sym.shift)
/ loc: loc.08048424 (74)
|  .    0x08048424  loc.08048424:
|  .--> 0x08048424     8b4508           mov eax, [ebp+0x8] ; ebp+0x8 = strlen(chain)
|  |    0x08048427     890424           mov [esp], eax
|  |    0x0804842a     e811ffffff       call dword imp.strlen
|  |       ; imp.strlen()
|  |    0x0804842f     394584           cmp [ebp-0x7c], eax
|  |,=< 0x08048432     731c             jae loc.08048450
|  ||   0x08048434     8d4588           lea eax, [ebp-0x78]
|  ||   0x08048437     89c2             mov edx, eax
|  ||   0x08048439     035584           add edx, [ebp-0x7c]
|  ||   0x0804843c     8b4584           mov eax, [ebp-0x7c]
|  ||   0x0804843f     034508           add eax, [ebp+0x8]
|  ||   0x08048442     0fb600           movzx eax, byte [eax]
|  ||   0x08048445     2c03             sub al, 0x3
|  ||   0x08048447     8802             mov [edx], al
|  ||   0x08048449     8d4584           lea eax, [ebp-0x7c]
|  ||   0x0804844c     ff00             inc dword [eax]
|  `==< 0x0804844e     ebd4             jmp loc.08048424
|   |   ; CODE (JMP) XREF 0x08048432 (sym.shift)
/ loc: loc.08048450 (30)
|   |   0x08048450  loc.08048450:
|   `-> 0x08048450     8d4588           lea eax, [ebp-0x78]
|       0x08048453     034584           add eax, [ebp-0x7c]
|       0x08048456     c60000           mov byte [eax], 0x0
|       0x08048459     8d4588           lea eax, [ebp-0x78]
|       0x0804845c     89442404         mov [esp+0x4], eax
|       0x08048460     c70424e8850408   mov dword [esp], 0x80485e8
|       0x08048467     e8e4feffff       call dword imp.printf
|          ; imp.printf()
|       0x0804846c     c9               leave
\       0x0804846d     c3               ret
        ; ------------
```

прочитав ассемблерный код, обнаруживаем, что расшифровка на самом деле - "sub al, 0x3". напишем программу python:

```python
print(''.join([chr(ord(i)-0x3) for i in 'SdvvzrugRN$$$']))
print(''.join([chr(ord(i)-0x3) for i in 'LqydolgSdvvzrug$']))
```

проще `запустить` этот код расшифровки, то есть отладить его или эмулировать. Попытка использования эмулятора ESIL radare2 провалилась, он завис при выполнении `call dword imp.strlen`. Также не удалось воспользоваться функцией hooking / пропуском инструкции в radare2.  Ниже приведен пример эмуляции ESIL.

```sh
[0x08048414]> s 0x08048445		# the 'sub al, 0x03'
[0x08048445]> aei				# init VM
[0x08048445]> aeim				# init memory
[0x08048445]> aeip				# init ip
[0x08048445]> aer eax=0x41		# set eax=0x41 -- 'A'
[0x08048445]> aer				# show current value of regs
oeax = 0x00000000
eax = 0x00000041
ebx = 0x00000000
ecx = 0x00000000
edx = 0x00000000
esi = 0x00000000
edi = 0x00000000
esp = 0x00178000
ebp = 0x00178000
eip = 0x08048445
eflags = 0x00000000
[0x08048445]> V					# enter Visual mode
# 'p' or 'P' to change visual mode
# I prefer the [xaDvc] mode
# use 's' to step in and 'S' to step over
[0x08048442 [xaDvc]0 0% 265 ./crackme0x03]> diq;?0;f t.. @ sym.shift+46 # 0x8048442
dead at 0x00000000
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x00178000  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00178010  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00178020  0000 0000 0000 0000 0000 0000 0000 0000  ................
0x00178030  0000 0000 0000 0000 0000 0000 0000 0000  ................
   oeax 0x00000000      eax 0x00000041      ebx 0x00000000      ecx 0x00000000
    edx 0x00000000      esi 0x00000000      edi 0x00000000      esp 0x00178000
    ebp 0x00178000      eip 0x08048445   eflags 0x00000000
       :   0x08048442      0fb600         movzx eax, byte [eax]
       :   ;-- eip:
       :   0x08048445      2c03           sub al, 3
       :   0x08048447      8802           mov byte [edx], al
       :   0x08048449      8d4584         lea eax, [var_7ch]
       :   0x0804844c      ff00           inc dword [eax]
       :=< 0x0804844e      ebd4           jmp 0x8048424
           ; CODE XREF from sym.shift @ 0x8048432
           0x08048450      8d4588         lea eax, [var_78h]
```

Кстати, можно открыть файл и использовать команду "write data" для расшифровки данных.

```sh
r2 -w ./crackme0x03
[0x08048360]> aaa
[0x08048360]> fs strings
[0x08048360]> f
0x080485ec 18 str.Lqydolg_Sdvvzrug
0x080485fe 18 str.Sdvvzrug_RN
0x08048610 25 str.IOLI_Crackme_Level_0x03
0x08048629 11 str.Password:
[0x08048360]> s str.Lqydolg_Sdvvzrug
[0x080485ec]> wos 0x03 @ str.Lqydolg_Sdvvzrug!0x11
[0x080485ec]> px
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x080485ec  496e 7661 6c69 6420 5061 7373 776f 7264  Invalid Password
0x080485fc  2100 5364 7676 7a72 7567 2352 4e24 2424  !.Sdvvzrug#RN$$$
0x0804860c  233d 2c00 494f 4c49 2043 7261 636b 6d65  #=,.IOLI Crackme
0x0804861c  204c 6576 656c 2030 7830 330a 0050 6173   Level 0x03..Pas
0x0804862c  7377 6f72 643a 2000 2564 0000 0000 0000  sword: .%d......
[0x080485ec]> wos 0x03 @ str.Sdvvzrug_RN!17
[0x080485ec]> px
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0x080485ec  496e 7661 6c69 6420 5061 7373 776f 7264  Invalid Password
0x080485fc  2100 5061 7373 776f 7264 204f 4b21 2121  !.Password OK!!!
0x0804860c  203a 2900 494f 4c49 2043 7261 636b 6d65   :).IOLI Crackme
0x0804861c  204c 6576 656c 2030 7830 330a 0050 6173   Level 0x03..Pas
0x0804862c  7377 6f72 643a 2000 2564 0000 0000 0000  sword: .%d......
[0x080485ec]>
```

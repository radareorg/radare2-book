IOLI 0x00
=========

Первая задачка IOLI, самая простая.

```
$ ./crackme0x00
IOLI Crackme Level 0x00
Password: 1234
Invalid Password!
```

Первое, что проверяется - пароль, он может быть просто открытым текстом внутри файла. Если это так, то ломать дальше ничего не нужно: используем rabin2 с флагом -z для перечисления строк в двоичном файле.

```
$ rabin2 -z ./crackme0x00
[Strings]
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000568 0x08048568 24  25   .rodata ascii IOLI Crackme Level 0x00\n
1   0x00000581 0x08048581 10  11   .rodata ascii Password:
2   0x0000058f 0x0804858f 6   7    .rodata ascii 250382
3   0x00000596 0x08048596 18  19   .rodata ascii Invalid Password!\n
4   0x000005a9 0x080485a9 15  16   .rodata ascii Password OK :)\n
```

Что представляет собой полученный текст? - это заголовок, отображаемым при запуске приложения.

```
nth paddr      vaddr      len size section type  string
-------------------------------------------------------
0   0x00000568 0x08048568 24  25   .rodata ascii IOLI Crackme Level 0x00\n
```

Эта строка - текст приглашения для ввода пароля.

```
1   0x00000581 0x08048581 10  11   .rodata ascii Password:
```

Эта - вывод сообщения об ошибке при вводе неверного пароля.

```
3   0x00000596 0x08048596 18  19   .rodata ascii Invalid Password!\n
```

Здесь говорится, что пароль принят.

```
4   0x000005a9 0x080485a9 15  16   .rodata ascii Password OK :)\n
```

А это что? Это строка, но ее не было при запуске приложения.

```
2   0x0000058f 0x0804858f 6   7    .rodata ascii 250382
```

Давайте попробуем ее в качестве пароля.

```
$ ./crackme0x00
IOLI Crackme Level 0x00
Password: 250382
Password OK :)
```

Ну вот, теперь ясно, что 250382 - это и есть пароль, на этом взлом закончен.

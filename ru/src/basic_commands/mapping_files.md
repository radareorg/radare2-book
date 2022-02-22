## Отображение файлов

Подсистема ввода-вывода Radare позволяет сопоставлять содержимое файлов в то же пространство ввода-вывода, используемое для хранения загруженного двоичного файла. Новое содержимое может быть размещено по любым смещениями.

Команда `o` позволяет пользователю открыть файл, он отображается на смещение 0, если у него нет стандартного двоичного заголовка, карты памяти создаются в виртуальных адресах.

Иногда надо перебазировать двоичный файл, или, загрузить и отобразить файл по другому адресу.

При запуске r2 базовый адрес можно изменить флагом `-B` . Есть разница при открытии файлов с неизвестными заголовками, такими как booloader-ы, их нужно отображать при помощи флага `-m`, или указав его в качестве аргумента команде `o`.

Программа radare2 способна открывать файлы и отображать их части в любые места памяти, указывая такие атрибуты, как разрешения и имя. Это идеальный подход к воспроизведению среды, такой как core dump, сеанс отладки, а также загрузки и отображения в память всех библиотек, от которых зависит двоичный файл.

Открытие файлов (и их отображение) осуществляется с помощью команды `o` (open). Посмотрим справку:

```
[0x00000000]> o?
|Usage: o [com- ] [file] ([offset])
| o                         list opened files
| o-1                       close file descriptor 1
| o-!*                      close all opened files
| o--                       close all files, analysis, binfiles, flags, same as !r2 --
| o [file]                  open [file] file in read-only
| o+ [file]                 open file in read-write mode
| o [file] 0x4000 rwx       map file at 0x4000
| oa[-] [A] [B] [filename]  Specify arch and bits for given file
| oq                        list all open files
| o*                        list opened files in r2 commands
| o. [len]                  open a malloc://[len] copying the bytes from current offset
| o=                        list opened files (ascii-art bars)
| ob[?] [lbdos] [...]       list opened binary files backed by fd
| oc [file]                 open core file, like relaunching r2
| of [file]                 open file and map it at addr 0 as read-only
| oi[-|idx]                 alias for o, but using index instead of fd
| oj[?]                     list opened files in JSON format
| oL                        list all IO plugins registered
| om[?]                     create, list, remove IO maps
| on [file] 0x4000          map raw file at 0x4000 (no r_bin involved)
| oo[?]                     reopen current file (kill+fork in debugger)
| oo+                       reopen current file in read-write
| ood[r] [args]             reopen in debugger mode (with args)
| oo[bnm] [...]             see oo? for help
| op [fd]                   prioritize given fd (see also ob)
| ox fd fdx                 exchange the descs of fd and fdx and keep the mapping
```

Покажем простой шаблон:

```sh
$ rabin2 -l /bin/ls
[Linked libraries]
libselinux.so.1
librt.so.1
libacl.so.1
libc.so.6

4 libraries
```

Сопоставление файла:

```
[0x00001190]> o /bin/zsh 0x499999
```

Список отображением файлов:

```
[0x00000000]> o
- 6 /bin/ls @ 0x0 ; r
- 10 /lib/ld-linux.so.2 @ 0x100000000 ; r
- 14 /bin/zsh @ 0x499999 ; r
```

Показать шестнадцатеричные значения из /bin/zsh:

```
[0x00000000]> px @ 0x499999
```

Отмена отображения файлов с помощью команды `o-`. Передайте необходимый дескриптор файла в качестве аргумента:

```
[0x00000000]> o-14
```

Также можно просмотреть таблицу ascii со списком открытых файлов:

```
[0x00000000]> ob=
```

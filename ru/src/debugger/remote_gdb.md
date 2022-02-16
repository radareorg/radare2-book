# Отладка при помощи gdbserver-а

Radare2 позволяет удаленно отлаживать по протоколу gdb. Запускаем gdbserver и подключаемся к нему с помощью radare2. Синтаксис подключения:

```
$ r2 -d gdb://<host>:<port>
```

Обратите внимание, что следующая команда делает то же самое, r2 использует подключаемый модуль отладки, указанный в uri, если такой есть.

```
$ r2 -D gdb gdb://<host>:<port>
```

Плагин отладки можно изменить во время выполнения с помощью команд `dL` и `Ld`.

Или, если gdbserver работает в расширенном режиме, можно подключиться прямо к процессу на хосте:

```
$ r2 -d gdb://<host>:<port>/<pid>
```

Также возможно начать отладку после анализа файла с помощью команды `doof`, которая перебазирует (rebase) данные текущего сеанса при открытии gdb

```
[0x00404870]> doof gdb://<host>:<port>/<pid>
```

После подключения можно использовать стандартные команды отладки r2.

Radare2 еще не умеет загружать символы из gdbserver, копия двоичного файла должна быть локально представлена для загрузки символов. В случае, если символы не загружены даже если двоичный файл присутствует, можно попробовать указать путь с помощью `e dbg.exe.path`:

```
$ r2 -e dbg.exe.path=<path> -d gdb://<host>:<port>
```

Если символы загружаются по неправильному базовому адресу, можно попробовать указать и базовый адрес `e bin.baddr`:

```
$ r2 -e bin.baddr=<baddr> -e dbg.exe.path=<path> -d gdb://<host>:<port>
```

Обычно gdbserver сообщает максимальный поддерживаемый размер пакета. В противном случае, radare2 использует разумные значения по умолчанию. Можно указать максимальный размер пакета в переменной окружения `R2_GDB_PKTSZ` . Можно узнать и установить максимальный размер пакета во время сеанса с плагином ввода-вывода, `=!` .

```
$ export R2_GDB_PKTSZ=512
$ r2 -d gdb://<host>:<port>
= attach <pid> <tid>
Assuming filepath <path/to/exe>
[0x7ff659d9fcc0]> =!pktsz
packet size: 512 bytes
[0x7ff659d9fcc0]> =!pktsz 64
[0x7ff659d9fcc0]> =!pktsz
packet size: 64 bytes
```

Плагин ввода-вывода gdb предоставляет полезные команды, которые могут не подходить ни для одной стандартной команды rdare2. Получить список этих команд можно с помощью
`=!?`. (Напомним, `=!` получает доступ к базовому плагину ввода-вывода `system()`).

```
[0x7ff659d9fcc0]> =!?
Usage: =!cmd args
 =!pid             - show targeted pid
 =!pkt s           - send packet 's'
 =!monitor cmd     - hex-encode monitor command and pass to target interpreter
 =!rd              - show reverse debugging availability
 =!dsb             - step backwards
 =!dcb             - continue backwards
 =!detach [pid]    - detach from remote/detach specific pid
 =!inv.reg         - invalidate reg cache
 =!pktsz           - get max packet size used
 =!pktsz bytes     - set max. размер пакета в байтах
 =!exec_file [pid] - get file which was executed for current/specified pid
```

Заметим, что `=!dsb` и `=!dcb` доступны только в специальных реализациях gdbserver, таких как [rr Мозиллы](https://github.com/mozilla/rr), gdbserver по умолчанию не включает поддержку удаленной реверс-отладки.
Используйте `=!rd` чтобы посмотреть доступные возможности реверс-отладки.

Если есть интерес в отладке взаимодействия radare2-а с сервером gdbserver, полезно использовать `=!monitor set remote-debug 1` для включения журналирования пакетов протокола управления формата gdb консоли gdbserver-а, также `=!monitor set debug 1` - вывод сообщений отладки, приходящих от gdbserver от его консоли.

radare2 также реализует собственный gdbserver:

```
$ r2 -
[0x00000000]> =g?
|Usage:  =[g] [...] # gdb server
| gdbserver:
| =g port file [args]   listen on 'port' debugging 'file' using gdbserver
| =g! файл порта [args] такой же, как указано выше, но не сообщения протокола отладки (например, gdbserver --remote-debug)
```

Запускать его так:

```
$ r2 -
[0x00000000]> =g 8000 /bin/radare2 -
```

А затем подключайтесь к нему, как к любому gdbserver. Например при помощи radare2:

```
$ r2 -d gdb://localhost:8000
```


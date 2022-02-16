# Возможности удаленного доступа

Radare, как правило, запускается локально, но можно запускать серверный процесс и  контролировать его локальным radare2. Реализация управления использует подсистему ввода-вывода radare, которая абстрагирует доступ к system(), cmd() и все основные операции ввода-вывода для работы по сети.

Справка по командам, используемым для организации удаленного доступа к radare:

```
[0x00405a04]> =?
Usage:  =[:!+-=ghH] [...]   # соединиться с другим процессом r2

команды удаленного доступа:
| =                             list all open connections
| =<[fd] cmd                    send output of local command to remote fd
| =[fd] cmd                     exec cmd at remote 'fd' (last open is default one)
| =! cmd                        run command via r_io_system
| =+ [proto://]host:port        connect to remote host:port (*rap://, raps://, tcp://, udp://, http://)
| =-[fd]                        remove all hosts or host 'fd'
| ==[fd]                        open remote session with host 'fd', 'q' to quit
| =!=                           disable remote cmd mode
| !=!                           установка режима удаленной командной строки

servers:
| .:9000                        start the tcp server (echo x|nc ::1 9090 or curl ::1:9090/cmd/x)
| =:port                        start the rap server (o rap://9999)
| =g[?]                         start the gdbserver
| =h[?]                         start the http webserver
| =H[?]                         start the http webserver (and launch the web browser)

other:
| =&:port                       start rap server in background (same as '&_=h')
| =:host:port cmd               run 'cmd' command on remote server

examples:
| =+tcp://localhost:9090/       connect to: r2 -c.:9090 ./bin
| =+rap://localhost:9090/       connect to: r2 rap://:9090
| =+http://localhost:9090/cmd/  connect to: r2 -c'=h 9090' bin
| o rap://:9090/                start the rap server on tcp port 9090
```

Инструкции по удаленным возможностям radare2 отображаются списком поддерживаемых плагинов ввода-вывода: `radare2 -L`.

Типичный удаленный сеанс выглядит следующим образом:

На удаленном хосте 1:

```
$ radare2 rap://:1234
```

На удаленном хосте 2:

```
$ radare2 rap://:1234
```

На локальном хосте:

```
$ radare2 -
```

Добавление хостов

```
[0x004048c5]> =+ rap://<host1>:1234//bin/ls
Connected to: <host1> at port 1234
waiting... ok

[0x004048c5]> =
0 - rap://<host1>:1234//bin/ls
```

Можно открывать удаленные файлы в режиме отладки (или с помощью любого подключаемого модуля ввода-вывода), указывая URI при добавлении хостов:

```
[0x004048c5]> =+ =+ rap://<host2>:1234/dbg:///bin/ls
Connected to: <host2> at port 1234
waiting... ok
0 - rap://<host1>:1234//bin/ls
1 - rap://<host2>:1234/dbg:///bin/ls
```

Выполнение команд на хосте 1:

```
[0x004048c5]> =0 px
[0x004048c5]> = s 0x666
```

Открыть сеанс связи с хостом 2:

```
[0x004048c5]> ==1
fd:6> pi 1
...
fd:6> q
```

Удаление узлов и закрытие подключения:

```
[0x004048c5]> =-
```

Можно также перенаправлять вывод radare на TCP- или UDP-сервер, например, при помощи `nc -l`. Сначала добавьте сервер при помощи '=+ tcp://' или '=+ udp://', затем можно перенаправить выходные данные команды на сервер:

```
[0x004048c5]> =+ tcp://<host>:<port>/
Connected to: <host> at port <port>
5 - tcp://<host>:<port>/
[0x004048c5]> =<5 cmd...
```

Команда `=<` отправит вывод команды `cmd` на удаленное подключение с номером N или на последнее подключение, если идентификатор не указан.

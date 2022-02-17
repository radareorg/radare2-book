# Плагины ввода-вывода

Весь доступ к файлам, сети, отладчику и вводу/выводу другого типа представлен специальным уровнем абстракции, позволяющим radare обрабатывать все данные как файл.

Плагины ввода-вывода представляют операции открытия, чтения, записи и 'system' в виде виртуальных файловых систем. Можно заставить radare обрабатывать что угодно как простой файл. Примерами выступают сокетные соединения, удаленный сеанс radare, файлы, процессы, устройства, сеансы gdb.

Таким образом, когда radare считывает блок байтов, задача плагина ввода-вывода заключается в том, чтобы получить эти байты из своего источника, поместить их во внутренний буфер. Плагин ввода-вывода выбирается по URI открываемого файла. Примеры:

* URI отладки
```
$ r2 dbg:///bin/ls<br />
$ r2 pid://1927
```
* Удаленные сеансы
```
$ r2 rap://:1234<br />
$ r2 rap://<host>:1234//bin/ls
```
* Виртуальные буферы
```
$ r2 malloc://512<br />
сокращение для
$ r2 -
```
Получение списка плагинов ввода-вывода в radare - `radare2 -L`:
```
$ r2 -L
rw_  ar       открытие файлов ar/lib [ar|lib]://[file//path] (LGPL3)
rw_  bfdbg    отладчик для экзотического BrainFuck (bfdbg://path/to/file) (LGPL3)
rwd  bochs    подключение к отладчику BOCHS (LGPL3)
r_d  debug    встроенный отладчик (dbg:///bin/ls dbg://1388 pidof:// waitfor://) (LGPL3) v0.2.0 pancake
rw_  default  открытие локальных файлов, используя def_mmap:// (LGPL3)
rwd  gdb      подключение к gdbserver, 'qemu -s', gdb://localhost:1234 (LGPL3)
rw_  gprobe   открытие gprobe-соединения - gprobe:// (LGPL3)
rw_  gzip     чтение/запись файлов gzip (LGPL3)
rw_  http     запрос GET HTTP (http://rada.re/) (LGPL3)
rw_  ihex     интерпретация файла Intel HEX (ihex://eeproms.hex) (LGPL)
r__  mach     отладка архитектуры mach io-плагин (в этой версии не поддерживается) (LGPL)
rw_  malloc   выделение памяти (malloc://1024 hex://cd8090) (LGPL3)
rw_  mmap     открытие файлов, используя mmap:// (LGPL3)
rw_  null     плагин null-plugin (null://23) (LGPL3)
rw_  procpid  доступ в /proc/pid/mem, io-плагин (LGPL3)
rwd  ptrace   использование ptrace и /proc/pid/mem (если доступны) io-плагин (LGPL3)
rwd  qnx      подключение к инстанцу QNX pdebug, qnx://host:1234 (LGPL3)
rw_  r2k      доступ к  API ядра, io-плагин (r2k://) (LGPL3)
rw_  r2pipe   r2pipe io-плагин (MIT)
rw_  r2web    r2web io-клиент (r2web://cloud.rada.re/cmd/) (LGPL3)
rw_  rap      сетевой протокол radare (rap://:port rap://host:port/file) (LGPL3)
rw_  rbuf     RBuffer io-плагин: rbuf:// (LGPL)
rw_  self     чтение памяти из своего адресного пространства, 'self://' (LGPL3)
rw_  shm      ресурсы разделяемой памяти (shm://key) (LGPL3)
rw_  sparse   выделение разреженных буферов (sparse buffer) (sparse://1024 sparse://) (LGPL3)
rw_  tcp      загрузка файлов по TCP (listen или connect) (LGPL3)
rwd  windbg   подключение к отладчику KD (windbg://socket) (LGPL3)
rwd  winedbg  Wine-dbg io- и debug.io-плагины для r2 (MIT)
rw_  zip      открытие файлов zip [apk|ipa|zip|zipall]://[file//path] (BSD)
```

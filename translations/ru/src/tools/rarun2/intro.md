# Программа Rarun2

Rarun2 - инструмент, позволяющий настроить специальную среду выполнения - переопределять stdin/stdout, каналы, переменные среды, а также параметров, полезных для формирования ограниченной среды, в которой предполагается выполнить двоичный файл, включая режим отладки.

```
$ rarun2 -h
Usage: rarun2 -v|-t|script.rr2 [directive ..]
```

Rarun2 используется как в качестве отдельного инструмента, так и в составе radare2. Задания среды исполнения (профиля) используется текстовый файл в формате ключ=значение. Формат профиля очень прост. Наиболее важные ключи - `program` и `arg*`. Профиль rarun2 можно загружать и в radare2. Для загрузки профиля из файла необходимо использовать флаг`-r`, флаг `-R` позволяет задавать директивы в командной строке.

Один из наиболее распространенных вариантов использования - перенаправление вывода отлаживаемой программы в radare2. Для этого нужно использовать `stdio`, `stdout`, `stdin`, `input` и пару аналогичных ключей.

Вот пример профиля:

```
program=/bin/ls
arg1=/bin
# arg2=hello
# arg3="hello\nworld"
# arg4=:048490184058104849
# arg5=:!ragg2 -p n50 -d 10:0x8048123
# arg6=@arg.txt
# arg7=@300@ABCD # 300 chars filled with ABCD pattern
# system=r2 -
# aslr=no
setenv=FOO=BAR
# unsetenv=FOO
# clearenv=true
# envfile=environ.txt
timeout=3
# timeoutsig=SIGTERM # or 15
# connect=localhost:8080
# listen=8080
# pty=false
# fork=true
# bits=32
# pid=0
# pidfile=/tmp/foo.pid
# #sleep=0
# #maxfd=0
# #execve=false
# #maxproc=0
# #maxstack=0
# #core=false
# #stdio=blah.txt
# #stderr=foo.txt
# stdout=foo.txt
# stdin=input.txt # or !program to redirect input from another program
# input=input.txt
# chdir=/
# chroot=/mnt/chroot
# libpath=$PWD:/tmp/lib
# r2preload=yes
# preload=/lib/libfoo.so
# setuid=2000
# seteuid=2000
# setgid=2001
# setegid=2001
# nice=5
```

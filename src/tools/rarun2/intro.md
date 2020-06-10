# Rarun2

Rarun2 is a tool allowing to setup a specified execution environment - redefine stdin/stdout, pipes,
change the environment variables and other settings useful to craft the boundary conditions you need to run
a binary for debugging.

```
$ rarun2 -h
Usage: rarun2 -v|-t|script.rr2 [directive ..]
```

It takes the text file in key=value format to specify the execution environment.
Rarun2 can be used as both separate tool or as a part of radare2.
To load the rarun2 profile in radare2 you need to use either `-r` to load
the profile from file or `-R` to specify the directive from string.

The format of the profile is very simple. Note the most important keys - `program` and `arg*`

One of the most common usage cases - redirect the output of debugged program in radare2.
For this you need to use `stdio`, `stdout`, `stdin`, `input`, and a couple similar keys.

Here is the basic profile example:

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

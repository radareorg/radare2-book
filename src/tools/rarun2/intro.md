## Rarun2

Rarun2 is a tool allowing to setup a specified execution environment - redefine stdin/stdout, pipes, change the environment variables and other settings useful to craft the boundary conditions you need to run a binary for debugging.

```console
$ rarun2 -h
Usage: rarun2 -v|-t|script.rr2 [directive ..]
```

It takes the text file in key=value format to specify the execution environment.  Rarun2 can be used as both separate tool or as a part of radare2.

To load the rarun2 profile in radare2 you need to use either `-r` to load the profile from file or `-R` to specify the directive from string.

The format of the profile is very simple. Note the most important keys - `program` and `arg*`

One of the most common usage cases - redirect the output of debugged program in radare2.  For this you need to use `stdio`, `stdout`, `stdin`, `input`, and a couple similar keys.

Here is the basic profile example:

```sh
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

### Sample rarun2 script

When this script is executed with rarun2, it will:

* Run the program "./pp400"
* Pass "10" as the first argument
* Use the contents of "foo.txt" as standard input
* Change the working directory to "/tmp" before execution

This setup is often used for debugging, testing, or analyzing programs in a controlled environment, especially in the context of reverse engineering or security research.

```console
$ cat foo.rr2
#!/usr/bin/rarun2
program=./pp400
arg0=10
stdin=foo.txt
chdir=/tmp
#chroot=.
./foo.rr2
```

### Using a program via TCP/IP

```console
$ nc -l 9999
$ rarun2 program=/bin/ls connect=localhost:9999
```

### Debugging a Program Redirecting the stdio into Another Terminal

1 - open a new terminal and type 'tty' to get a terminal name:

```console
$ tty ; clear ; sleep 999999
/dev/ttyS010
```

2 - Create a new file containing the following rarun2 profile named foo.rr2:

```sh
#!/usr/bin/rarun2
program=/bin/ls
stdio=/dev/ttys010
```

3 - Launch the following radare2 command:

```console
$ r2 -r foo.rr2 -d /bin/ls
```

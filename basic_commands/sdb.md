# SDB

SDB stands for String DataBase. It's a simple key-value database that only operates with strings created by pancake. It is used in many parts of r2 to have a disk and in-memory database which is small and fast to manage using it as a hashtable on steroids.

SDB is a simple string key/value database based on djb’s cdb disk storage and supports JSON and arrays introspection.

There’s also the sdbtypes: a vala library that implements several data structures on top of an sdb or a memcache instance.

SDB supports:
    
- namespaces (multiple sdb paths)
- atomic database sync (never corrupted)
- bindings for vala, luvit, newlisp and nodejs
- commandline frontend for sdb databases
- memcache client and server with sdb backend
- arrays support (syntax sugar)
- json parser/getter


## Usage example
Let's create a database!

```
$ sdb d hello=world
$ sdb d hello
world
```

Using arrays:
```
$ sdb - '[]list=1,2' '[0]list' '[0]list=foo' '[]list' '[+1]list=bar'
1
foo
2
foo
bar
2
```

Let's play with json:
```
$ sdb d g='{"foo":1,"bar":{"cow":3}}'
$ sdb d g?bar.cow
3
$ sdb - user='{"id":123}' user?id=99 user?id
99
```

Using the command line without any disk database:
```
$ sdb - foo=bar foo a=3 +a -a
bar
4
3

$ sdb -
foo=bar
foo
bar
a=3
+a
4
-a
3
```
Remove the database
```
$ rm -f d

```

## So what ?
So, you can now do this inside your radare2 sessions!

Let's take a simple binary, and check what is already _sdbized_.
```
$ cat test.c
int main(){
	puts("Hello world\n");
}
$ gcc test.c -o test
```

```
$ r2 -A ./test
[0x08048320]> k **
bin
anal
syscall
debug
```

```
[0x08048320]> k bin/**
fd.6
[0x08048320]> k bin/fd.6/*
archs=0:0:x86:32
```
The file corresponding to the sixth file descriptor is a x86_32 binary. 

```
[0x08048320]> k anal/meta/*
meta.s.0x80484d0=12,SGVsbG8gd29ybGQ=
[...]
[0x08048320]> ?b64- SGVsbG8gd29ybGQ=
Hello world
```
Strings are stored encoded in base64.

---

## More Examples


List namespaces
```
k **
```
List sub-namespaces
```
k anal/**
```
List keys
```
k *
k anal/*
```
Set a key
```
k foo=bar
```
Get the value of a key
```
k foo
```

List all syscalls
```
k syscall/*~^0x
```
List all comments
```
k anal/meta/*~.C.
```
Show a comment at given offset:
```
k %anal/meta/[1]meta.C.0x100005000
```

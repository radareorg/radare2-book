# SDB

SDB stands for String DataBase. It's a simple key-value database that only operates with strings created by pancake. It is used in many parts of r2 to have a disk and in-memory database which is small and fast to manage using it as a hashtable with asteroids.

The k command give us the interface to run sdb_query expressions to read or change internal information from the analysis, syscalls, binary information, etc

Here there are some usage examples:

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

Useful queries
--------------

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

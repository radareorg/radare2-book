# SDB

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
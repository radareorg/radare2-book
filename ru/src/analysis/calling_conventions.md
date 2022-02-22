# Соглашения вызова функции

Radare2 использует соглашения о вызове, чтобы помочь в определении формальных аргументов функции и возвращаемых типов.
Он также используется в качестве логической основы для базового прототипирования функций и вывода типа.

```
[0x00000000]> afc?
|Usage: afc[agl?]
| afc convention  Manually set calling convention for current function
| afc             Show Calling convention for the Current function
| afc=([cctype])  Select or show default calling convention
| afcr[j]         Show register usage for the current function
| afca            Analyse function for finding the current calling convention
| afcf[j] [name]  Prints return type function(arg1, arg2...), see afij
| afck            List SDB details of call loaded calling conventions
| afcl            List all available calling conventions
| afco path       Open Calling Convention sdb profile from given path
| afcR            Register telescoping using the calling conventions order
[0x00000000]>
```

* Вывод списка всех доступных соглашений о вызовах для текущей архитектуры - команда `afcl`

```
[0x00000000]> afcl
amd64
ms
```
* Для отображения прототипа функции стандартной библиотеки у вас есть команда `afcf`

```
[0x00000000]> afcf printf
int printf(const char *format)
[0x00000000]> afcf fgets
char *fgets(char *s, int size, FILE *stream)
```

Вся эта информация загружается через sdb в `/libr/anal/d/cc-[arch]-[bits].sdb`

```
default.cc=amd64

ms=cc
cc.ms.name=ms
cc.ms.arg1=rcx
cc.ms.arg2=rdx
cc.ms.arg3=r8
cc.ms.arg3=r9
cc.ms.argn=stack
cc.ms.ret=rax
```

`cc.x.argi=rax` используется для задания i-го аргумента функции, регистрации имени `rax`

`cc.x.argn=stack` означает, что все аргументы (или остальные аргументы в случае наличия argi для любого i) будут храниться в стеке слева направо

`cc.x.argn=stack_rev` то же, что `и cc.x.argn=stack`, но аргументы передаются справа налево

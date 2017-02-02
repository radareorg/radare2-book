Apart from simple sequencing and looping, radare2 allows to write
simple macros, using this construction:
```
[0x00404800]> (qwe, pd 4, ao)
```

This will define a macro called 'qwe' which runs sequentially first 'pd 4' then 'ao'.
Calling the macro using syntax `.(macro)` is simple:

```
[0x00404800]> (qwe, pd 4, ao)
[0x00404800]> .(qwe)
	0x00404800      b827e66100     mov eax, 0x61e627      ; "tab"
	0x00404805      55             push rbp
	0x00404806      482d20e66100   sub rax, section_end.LOAD1
	0x0040480c      4889e5         mov rbp, rsp

address: 0x404800
opcode: mov eax, 0x61e627
prefix: 0
bytes: b827e66100
ptr: 0x0061e627
refptr: 0
size: 5
type: mov
esil: 6415911,rax,=
stack: null
family: cpu
[0x00404800]>
```

To list available macroses simply call `(*`:
```
[0x00404800]> (*
(qwe , pd 4, ao)
```

And if want to remove some macro, just add '-' before the name:
```
[0x00404800]> (-qwe)
Macro 'qwe' removed.
[0x00404800]>
```

Moreover, it's possible to create a macro that takes arguments, which comes in handy in some
simple scripting situations. To create a macro that takes arguments you simply add them to macro definition. Be sure, if you're using characters like ';', to quote the whole command for proper parsing.

```
[0x00404800]
[0x004047d0]> "(foo x y,pd $0; s +$1)"
[0x004047d0]> .(foo 5 6)
		;-- entry0:
		0x004047d0      31ed           xor ebp, ebp
		0x004047d2      4989d1         mov r9, rdx
		0x004047d5      5e 			   pop rsi
		0x004047d6		4889e2         mov rdx, rsp
		0x004047d9		4883e4f0       and rsp, 0xfffffffffffffff0
[0x004047d6]>
```
As you can see, the arguments are named by index, starting from 0: $0, $1, ...


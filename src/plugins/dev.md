## Implementing a new architecture

radare2 splits the logic of a CPU into several modules. You should write more than one plugin to get full support for a specific arch. Let's see which are those:

* r_asm : assembler and disassembler
* r_anal : code analysis (opcode,type,esil,..)
* r_reg : registers
* r_syscall : system calls
* r_debug : debugger

The most basic feature you usually want to support from a specific architecture is the disassembler. You first need to read into a human readable form the bytes in there.

Bear in mind that plugins can be compiled static or dynamically, this means that the arch will be embedded inside the core libraries or it will distributed as a separated shared library.

To configure which plugins you want to compile use the `./configure-plugins` script which accepts the flags --shared and --static to specify them. You can also add it manually inside the `plugins.def.cfg` and then remove the `plugins.cfg` and run `./configure-plugins` again to update the `libr/config.mk` and `libr/config.h`.

You may find some examples of external plugins in [radare2-extras](https://github.com/radareorg/radare2-extras) repository.

## Writing the r_asm plugin

The official way to make third-party plugins is to distribute them into a separate repository. This is a sample disasm plugin:

```Makefile
$ cd my-cpu
$ cat Makefile
NAME=mycpu
R2_PLUGIN_PATH=$(shell r2 -hh|grep R2_LIBR_PLUGINS|awk '{print $$2}')
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_asm)
LDFLAGS=-shared $(shell pkg-config --libs r_asm)
OBJS=$(NAME).o
SO_EXT=$(shell uname|grep -q Darwin && echo dylib || echo so)
LIB=$(NAME).$(SO_EXT)

all: $(LIB)

clean:
	rm -f $(LIB) $(OBJS)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIB)

install:
	cp -f $(NAME).$(SO_EXT) $(R2_PLUGIN_PATH)

uninstall:
	rm -f $(R2_PLUGIN_PATH)/$(NAME).$(SO_EXT)
```

```c
$ cat mycpu.c
/* example r_asm plugin by pancake at 2014 */

#include <r_asm.h>
#include <r_lib.h>

#define OPS 17

static const char *ops[OPS*2] = {
	"nop", NULL,
	"if", "r",
	"ifnot", "r",
	"add", "rr",
	"addi", "ri",
	"sub", "ri",
	"neg", "ri",
	"xor", "ri",
	"mov", "ri",
	"cmp", "rr",
	"load", "ri",
	"store", "ri",
	"shl", "ri",
	"br", "r",
	"bl", "r",
	"ret", NULL,
	"sys", "i"
};

/* Main function for disassembly */
//b for byte, l for length
static int disassemble (RAsm *a, RAsmOp *op, const ut8 *b, int l) {
	char arg[32];
        int idx = (b[0]&0xf)\*2;
	op->size = 2;
	if (idx>=(OPS*2)) {
		strcpy (op->buf_asm, "invalid");
		return -1;
	}
	strcpy (op->buf_asm, ops[idx]);
	if (ops[idx+1]) {
		const char \*p = ops[idx+1];
		arg[0] = 0;
		if (!strcmp (p, "rr")) {
			sprintf (arg, "r%d, r%d", b[1]>>4, b[1]&0xf);
		} else
		if (!strcmp (p, "i")) {
			sprintf (arg, "%d", (char)b[1]);
		} else
		if (!strcmp (p, "r")) {
			sprintf (arg, "r%d, r%d", b[1]>>4, b[1]&0xf);
		} else
		if (!strcmp (p, "ri")) {
			sprintf (arg, "r%d, %d", b[1]>>4, (char)b[1]&0xf);
		}
		if (*arg) {
			strcat (op->buf_asm, " ");
			strcat (op->buf_asm, arg);
		}
	}
	return op->size;
}

/* Structure of exported functions and data */
RAsmPlugin r_asm_plugin_mycpu = {
        .name = "mycpu",
        .arch = "mycpu",
        .license = "LGPL3",
        .bits = 32,
        .desc = "My CPU disassembler",
        .disassemble = &disassemble,
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
        .type = R_LIB_TYPE_ASM,
        .data = &r_asm_plugin_mycpu
};
#endif
```

To build and install this plugin just type this:

```
$ make
$ sudo make install
```

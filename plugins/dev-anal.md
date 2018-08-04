## Implementing a new analysis plugin

After implementing disassembly plugin, you might have noticed that output
is far from being good - no proper highlighting, no reference lines
and so on. This is because radare2 requires every architecture plugin
to provide also analysis information about every opcode. At the moment
the implementation of disassembly and opcodes analysis is separated between
two modules - RAsm and RAnal. Thus we need to write an analysis plugin too.
The principle is very similar - you just need to create a C file and
corresponding Makefile.

**Makefile**

```makefile
NAME=anal_snes
R2_PLUGIN_PATH=$(shell r2 -H|grep USER_PLUGINS|awk '{print $$2}')
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_anal)
LDFLAGS=-shared $(shell pkg-config --libs r_anal)
OBJS=$(NAME).o
SO_EXT=$(shell uname|grep -q Darwin && echo dylib || echo so)
LIB=$(NAME).$(SO_EXT)

all: $(LIB)

clean:
	rm -f $(LIB) $(OBJS)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIB)

install:
	cp -f anal_snes.$(SO_EXT) $(R2_PLUGIN_PATH)

uninstall:
	rm -f $(R2_PLUGIN_PATH)/anal_snes.$(SO_EXT)
```

**anal_snes.c:**

```c
/* radare - LGPL - Copyright 2015 - condret */

#include <string.h>
#include <r_types.h>
#include <r_lib.h>
#include <r_asm.h>
#include <r_anal.h>
#include "snes_op_table.h"

static int snes_anop(RAnal *anal, RAnalOp *op, ut64 addr, const ut8 *data, int len) {
	memset (op, '\0', sizeof (RAnalOp));
	op->size = snes_op[data[0]].len;
	op->addr = addr;
	op->type = R_ANAL_OP_TYPE_UNK;
	switch (data[0]) {
		case 0xea:
			op->type = R_ANAL_OP_TYPE_NOP;
			break;
	}
	return op->size;
}

struct r_anal_plugin_t r_anal_plugin_snes = {
	.name = "snes",
	.desc = "SNES analysis plugin",
	.license = "LGPL3",
	.arch = R_SYS_ARCH_NONE,
	.bits = 16,
	.init = NULL,
	.fini = NULL,
	.op = &snes_anop,
	.set_reg_profile = NULL,
	.fingerprint_bb = NULL,
	.fingerprint_fcn = NULL,
	.diff_bb = NULL,
	.diff_fcn = NULL,
	.diff_eval = NULL
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
	.type = R_LIB_TYPE_ANAL,
	.data = &r_anal_plugin_snes,
	.version = R2_VERSION
};
#endif
```
After compiling radare2 will list this plugin in the output:
```
_dA_  _8_16      snes        LGPL3   SuperNES CPU
```

**snes_op_table**.h: https://github.com/radare/radare2/blob/master/libr/asm/arch/snes/snes_op_table.h

Example:

* **6502**: https://github.com/radare/radare2/commit/64636e9505f9ca8b408958d3c01ac8e3ce254a9b
* **SNES**: https://github.com/radare/radare2/commit/60d6e5a1b9d244c7085b22ae8985d00027624b49


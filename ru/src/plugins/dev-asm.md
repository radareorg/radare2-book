## Реализация плагина дизассемблирования

Архитектура Radare2 модульная, поэтому реализация поддержки новой архитектуры очень проста, особенно тем, кто свободно владеет С. По множеству причин реализацию еще проще сделать на основе исходников из дерева репозитория radare2. 

Создаем всего один файл C, называемый `asm_mycpu.c` и соответсвующий Makefile.

Ключевым моментом плагина RAsm является структура
```c
RAsmPlugin r_asm_plugin_mycpu = {
	.name = "mycpu",
	.license = "LGPL3",
	.desc = "MYCPU disassembly plugin",
	.arch = "mycpu",
	.bits = 32,
	.endian = R_SYS_ENDIAN_LITTLE,
	.disassemble = &disassemble
};
```

Здесь `.disassemble` — это указатель на функцию дизассемблирования,  принимающую буфер байтов и его длину:

```c
static int disassemble(RAsm *a, RAsmOp *op, const ut8 *buf, int len)
```

**Makefile**

```makefile
NAME=asm_snes
R2_PLUGIN_PATH=$(shell r2 -H R2_USER_PLUGINS)
LIBEXT=$(shell r2 -H LIBEXT)
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_anal)
LDFLAGS=-shared $(shell pkg-config --libs r_anal)
OBJS=$(NAME).o
LIB=$(NAME).$(LIBEXT)

all: $(LIB)

clean:
	rm -f $(LIB) $(OBJS)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(LIB)

install:
	cp -f asm_mycpu.$(SO_EXT) $(R2_PLUGIN_PATH)

uninstall:
	rm -f $(R2_PLUGIN_PATH)/asm_mycpu.$(SO_EXT)
```

**asm_mycpu.c**

```c
/* radare - LGPL - Copyright 2018 - user */

#include <stdio.h>
#include <string.h>
#include <r_types.h>
#include <r_lib.h>
#include <r_asm.h>

static int disassemble(RAsm *a, RAsmOp *op, const ut8 *buf, int len) {
	struct op_cmd cmd = {
		.instr = "",
		.operands = ""
	};
	if (len < 2) return -1;
	int ret = decode_opcode (buf, len, &cmd);
	if (ret > 0) {
		snprintf (op->buf_asm, R_ASM_BUFSIZE, "%s %s",
			  cmd.instr, cmd.operands);
	}
	return op->size = ret;
}

RAsmPlugin r_asm_plugin_mycpu = {
	.name = "mycpu",
	.license = "LGPL3",
	.desc = "MYCPU disassembly plugin",
	.arch = "mycpu",
	.bits = 32,
	.endian = R_SYS_ENDIAN_LITTLE,
	.disassemble = &disassemble
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_ASM,
	.data = &r_asm_plugin_mycpu,
	.version = R2_VERSION
};
#endif
```

После компиляции radare2 упомянет плагин в списке плагинов вывода:
```
_d__  _8_32      mycpu        LGPL3   MYCPU
```

### Перемещение плагина в дерево исходников radare2

Добавление новой архитектуры в основную ветвь r2 требует изменения нескольких файлов, что обеспечивает плагину правильную интеграцию аналогично остальным плагинам.

Список файлов, требующих изменения:

* `plugins.def.cfg` : добавить название плагина в виде строки `asm.mycpu`,
* `libr/asm/p/mycpu.mk` : инструкции сборки,
* `libr/asm/p/asm_mycpu.c` : реализация,
* `libr/include/r_asm.h` : добавление определенных структур.

Сравните результат с плагином дизассемблирования для процессора NIOS II по следующим коммитам:

Implement RAsm plugin:
https://github.com/radareorg/radare2/commit/933dc0ef6ddfe44c88bbb261165bf8f8b531476b

Реализация плагина RAnal:
https://github.com/radareorg/radare2/commit/ad430f0d52fbe933e0830c49ee607e9b0e4ac8f2


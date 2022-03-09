## Реализация нового плагина анализа

После реализации плагина дизассемблирования, его вывод бывает далеко не идеальным - нет правильной подсветки, нет "опорных линий"
и т.д. Radare2 требует от каждого архитектурного плагина предоставлять также аналитическую информацию о каждом оп-коде. На данный момент реализация дизассемблирования и анализа оп-кодов разделена на два модуля - RAsm и RAnal. 

Реализуем плагин анализа. Принцип схож с реализацией дизассемблирования - реализовать C-файл и соответствующий Makefile.

Структура плагина RAnal выглядит так -

```c
RAnalPlugin r_anal_plugin_v810 = {
	.name = "mycpu",
	.desc = "MYCPU code analysis plugin",
	.license = "LGPL3",
	.arch = "mycpu",
	.bits = 32,
	.op = mycpu_op,
	.esil = true,
	.set_reg_profile = set_reg_profile,
};
```

Аналогично плагину дизассемблирования, есть ключевая функция - `mycpu_op`, сканирующая оп-код и создающая структуру RAnalOp. В этом примере плагины анализа также выполняют описание оп-кода командами ESIL (uplifting), так как включен `.esil = true`. Таким образом, `mycpu_op` обязан заполнять соответствующее ESIL-поле структуры RAnalOp для оп-кодов. Вторая важная вещь для кодирования оп-кодов в ESIL и выполнения эмуляции - регистровый профиль, подобно тому, как в отладчике, он задается внутри функции `set_reg_profile`.

**Makefile**

```makefile
NAME=anal_snes
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

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_ANAL,
	.data = &r_anal_plugin_snes,
	.version = R2_VERSION
};
#endif
```
После компиляции radare2 упомянет плагин в списке плагинов вывода:
```
_dA_  _8_16      snes        LGPL3   SuperNES CPU
```

**snes_op_table**.h: https://github.com/radareorg/radare2/blob/master/libr/asm/arch/snes/snes_op_table.h

Пример:

* **6502**: https://github.com/radareorg/radare2/commit/64636e9505f9ca8b408958d3c01ac8e3ce254a9b
* **SNES**: https://github.com/radareorg/radare2/commit/60d6e5a1b9d244c7085b22ae8985d00027624b49

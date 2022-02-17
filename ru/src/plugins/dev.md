## Реализация новой архитектуры

radare2 разделяет логику описания процессора на несколько модулей. Нужно реализовать несколько плагинов, чтобы получить полную поддержку для конкретной архитектуры. Посмотрим, какие они:

* r_asm : ассемблер и дизассемблер
* r_anal : анализ кода (оп-код, типы, esil, ...)
* r_reg : регистры
* r_syscall : системные вызовы
* r_debug : отладчик

Основной функцией, поддерживаемой для определенной архитектуры, является дизассемблер. Сначала нужно представить байты в форме, воспринимаемой человеком.

Плагины компилируются как статически, так и динамически, т.е. поддержка архитектуры будет или встроена в ядро или будет динамической библиотекой.

Чтобы настроить перечень плагинов для сборки используйте скрипт `./configure-plugins`, принимающий флаги --shared и --static. Можете перечислить их вручную внутри `plugins.def.cfg` затем удалить `plugins.cfg`, запустить `./configure-plugins` для обновления `libr/config.mk` и `libr/config.h`.

Есть примеры внешних плагинов в репозитории [radare2-extras](https://github.com/radareorg/radare2-extras) .

## Реализация плагина r_asm

Официальным способом создания плагинов является их публикация в отдельном репозиторий. Пример плагина дизассемблирования:

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

Соборка и установка плагина - команды

```
$ make
$ sudo make install
```

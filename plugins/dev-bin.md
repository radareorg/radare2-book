## Implementing a new format

### To enable virtual addressing

In `info` add `et->has_va = 1;` and `ptr->srwx` with the `R_BIN_SCN_MAP;` attribute

### Create a folder with file format name in libr/bin/format

**Makefile:**

```Makefile
NAME=bin_nes
R2_PLUGIN_PATH=$(shell r2 -hh|grep R2_LIBR_PLUGINS|awk '{print $$2}')
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_bin)
LDFLAGS=-shared $(shell pkg-config --libs r_bin)
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

**bin_nes.c:**

```c
#include <r_bin.h>

static int check(RBinFile *arch);
static int check_bytes(const ut8 *buf, ut64 length);

static void * load_bytes(RBinFile *arch, const ut8 *buf, ut64 sz, ut64 loadaddr, Sdb *sdb){
	check_bytes (buf, sz);
	return R_NOTNULL;
}


static int check(RBinFile *arch) {
	const ut8 \*bytes = arch ? r_buf_buffer (arch->buf) : NULL;
	ut64 sz = arch ? r_buf_size (arch->buf): 0;
	return check_bytes (bytes, sz);
}

static int check_bytes(const ut8 *buf, ut64 length) {
	if (!buf || length < 4) return false;
	return (!memcmp (buf, "\x4E\x45\x53\x1A", 4));
}


static RBinInfo* info(RBinFile *arch) {
	RBinInfo \*ret = R_NEW0 (RBinInfo);
	if (!ret) return NULL;

	if (!arch || !arch->buf) {
		free (ret);
		return NULL;
	}
	ret->file = strdup (arch->file);
	ret->type = strdup ("ROM");
	ret->machine = strdup ("Nintendo NES");
	ret->os = strdup ("nes");
	ret->arch = strdup ("6502");
	ret->bits = 8;

	return ret;
}


struct r_bin_plugin_t r_bin_plugin_nes = {
	.name = "nes",
	.desc = "NES",
	.license = "BSD",
	.init = NULL,
	.fini = NULL,
	.get_sdb = NULL,
	.load = NULL,
	.load_bytes = &load_bytes,
	.check = &check,
	.baddr = NULL,
	.check_bytes = &check_bytes,
	.entries = NULL,
	.sections = NULL,
	.info = &info,
};

#ifndef CORELIB
struct r_lib_struct_t radare_plugin = {
	.type = R_LIB_TYPE_BIN,
	.data = &r_bin_plugin_nes,
	.version = R2_VERSION
};
#endif

```

### Some Examples

* XBE - https://github.com/radare/radare2/pull/972
* COFF - https://github.com/radare/radare2/pull/645
* TE - https://github.com/radare/radare2/pull/61
* Zimgz - https://github.com/radare/radare2/commit/d1351cf836df3e2e63043a6dc728e880316f00eb
* OMF - https://github.com/radare/radare2/commit/44fd8b2555a0446ea759901a94c06f20566bbc40


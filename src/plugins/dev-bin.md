## Implementing a new format

### To enable virtual addressing

In `info` add `et->has_va = 1;` and `ptr->srwx` with the `R_BIN_SCN_MAP;` attribute

### Create a folder with file format name in libr/bin/format

**Makefile:**

```Makefile
NAME=bin_nes
R2_PLUGIN_PATH=$(shell r2 -H R2_USER_PLUGINS)
LIBEXT=$(shell r2 -H LIBEXT)
CFLAGS=-g -fPIC $(shell pkg-config --cflags r_bin)
LDFLAGS=-shared $(shell pkg-config --libs r_bin)
OBJS=$(NAME).o
LIB=$(NAME).$(LIBEXT)

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
#include <r_util.h>
#include <r_bin.h>

static bool load_buffer(RBinFile *bf, void **bin_obj, RBuffer *b, ut64 loadaddr, Sdb *sdb) {
	ut64 size;
	const ut8 *buf = r_buf_data (b, &size);
	r_return_val_if_fail (buf, false);
	*bin_obj = r_bin_internal_nes_load (buf, size);
	return *bin_obj != NULL;
}

static void destroy(RBinFile *bf) {
	r_bin_free_all_nes_obj (bf->o->bin_obj);
	bf->o->bin_obj = NULL;
}

static bool check_buffer(RBuffer *b) {
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
	.get_sdb = NULL,
	.load_buffer = &load_buffer,
	.destroy = &destroy,
	.check_buffer = &check_buffer,
	.baddr = NULL,
	.entries = NULL,
	.sections = NULL,
	.info = &info,
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
	.type = R_LIB_TYPE_BIN,
	.data = &r_bin_plugin_nes,
	.version = R2_VERSION
};
#endif

```

### Some Examples

* XBE - https://github.com/radareorg/radare2/pull/972
* COFF - https://github.com/radareorg/radare2/pull/645
* TE - https://github.com/radareorg/radare2/pull/61
* Zimgz - https://github.com/radareorg/radare2/commit/d1351cf836df3e2e63043a6dc728e880316f00eb
* OMF - https://github.com/radareorg/radare2/commit/44fd8b2555a0446ea759901a94c06f20566bbc40


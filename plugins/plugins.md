# Plugins

It is common to have an issues when you write a plugin, especially if you do this for the first
time. This is why debugging them is very important. The first step for debugging is
to set an environment variable when running radare2 instance:
```
R_DEBUG=yes r2 /bin/ls
Loading /usr/local/lib/radare2/2.2.0-git//bin_xtr_dyldcache.so
Cannot find symbol 'radare_plugin' in library '/usr/local/lib/radare2/2.2.0-git//bin_xtr_dyldcache.so'
Cannot open /usr/local/lib/radare2/2.2.0-git//2.2.0-git
Loading /home/user/.config/radare2/plugins/asm_mips_ks.so
PLUGIN OK 0x55b205ea6070 fcn 0x7f298de08762
Loading /home/user/.config/radare2/plugins/asm_sparc_ks.so
PLUGIN OK 0x55b205ea6070 fcn 0x7f298de08762
Cannot open /home/user/.config/radare2/plugins/pimp
Cannot open /home/user/.config/radare2/plugins/yara
Loading /home/user/.config/radare2/plugins/asm_arm_ks.so
PLUGIN OK 0x55b205ea6070 fcn 0x7f298de08762
Loading /home/user/.config/radare2/plugins/core_yara.so
Module version mismatch /home/user/.config/radare2/plugins/core_yara.so (2.1.0) vs (2.2.0-git)
Loading /home/user/.config/radare2/plugins/asm_ppc_ks.so
PLUGIN OK 0x55b205ea6070 fcn 0x7f298de08762
Loading /home/user/.config/radare2/plugins/lang_python3.so
PLUGIN OK 0x55b205ea5ed0 fcn 0x7f298de08692
Loading /usr/local/lib/radare2/2.2.0-git/bin_xtr_dyldcache.so
Cannot find symbol 'radare_plugin' in library '/usr/local/lib/radare2/2.2.0-git/bin_xtr_dyldcache.so'
Cannot open /usr/local/lib/radare2/2.2.0-git/2.2.0-git
Cannot open directory '/usr/local/lib/radare2-extras/2.2.0-git'
Cannot open directory '/usr/local/lib/radare2-bindings/2.2.0-git'
USER CONFIG loaded from /home/user/.config/radare2/radare2rc
 -- In visual mode press 'c' to toggle the cursor mode. Use tab to navigate
[0x00005520]>
```

## IO plugins

All access to files, network, debugger, etc. is wrapped by an IO abstraction layer that allows radare to treat all data as if it were just a file.

IO plugins are the ones used to wrap the open, read, write and 'system' on virtual file systems. You can make radare understand anything as a plain file. E.g., a socket connection, a remote radare session, a file, a process, a device, a gdb session, etc..

So, when radare reads a block of bytes, it is the task of an IO plugin to get these bytes from any place and put them into internal buffer. An IO plugin is chosen by a file's URI to be opened. Some examples:

* Debugging URIs

    $ r2 dbg:///bin/ls
    $ r2 pid://1927

* Remote sessions

    $ r2 rap://:1234
    $ r2 rap://<host>:1234//bin/ls

* Virtual buffers

    $ r2 malloc://512
    shortcut for
    $ r2 -

You can get a list of the radare IO plugins by typing `radare2 -L`:

    $ r2 -L
    rw_  zip         Open zip files apk://foo.apk//MANIFEST or zip://foo.apk//theclass/fun.class, show files with: zip://foo.apk/, open all files with zipall:// (BSD)
    rwd  windbg      Attach to a KD debugger (LGPL3)
    rw_  sparse      sparse buffer allocation (sparse://1024 sparse://) (LGPL3)
    rw_  shm         shared memory resources (shm://key) (LGPL3)
    rw_  self        read memory from myself using 'self://' (LGPL3)
    rw_  rap         radare network protocol (rap://:port rap://host:port/file) (LGPL3)
    rwd  ptrace      ptrace and /proc/pid/mem (if available) io (LGPL3)
    rw_  procpid     /proc/pid/mem io (LGPL3)
    rw_  mmap        open file using mmap:// (LGPL3)
    rw_  malloc      memory allocation (malloc://1024 hex://cd8090) (LGPL3)
    r__  mach        mach debug io (unsupported in this platform) (LGPL)
    rw_  ihex        Intel HEX file (ihex://eeproms.hex) (LGPL)
    rw_  http        http get (http://radare.org/) (LGPL3)
    rw_  gzip        read/write gzipped files (LGPL3)
    rwd  gdb         Attach to gdbserver, 'qemu -s', gdb://localhost:1234 (LGPL3)
    r_d  debug       Debug a program or pid. dbg:///bin/ls, dbg://1388 (LGPL3)
    rw_  bfdbg       BrainFuck Debugger (bfdbg://path/to/file) (LGPL3)

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

You may find some examples of external plugins in [radare2-extras](https://github.com/radare/radare2-extras) repository.

## Writing the r_asm plugin

The official way to make third-party plugins is to distribute them into a separate repository. This is a sample disasm plugin:

```Makefile
$ cd my-cpu
$ cat Makefile
NAME=mycpu
R2_PLUGIN_PATH=$(shell r2 -hh|grep LIBR_PLUGINS|awk '{print $$2}')
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

## Testing the plugin

This plugin is used by rasm2 and r2. You can verify that the plugin is properly loaded with this command:
```
$ rasm2 -L | grep mycpu
_d  mycpu        My CPU disassembler  (LGPL3)
```

Let's open an empty file using the 'mycpu' arch and write some random code there.

```
$ r2 -
 -- I endians swap
[0x00000000]> e asm.arch=mycpu
[0x00000000]> woR
[0x00000000]> pd 10
           0x00000000    888e         mov r8, 14
           0x00000002    b2a5         ifnot r10, r5
           0x00000004    3f67         ret
           0x00000006    7ef6         bl r15, r6
           0x00000008    2701         xor r0, 1
           0x0000000a    9826         mov r2, 6
           0x0000000c    478d         xor r8, 13
           0x0000000e    6b6b         store r6, 11
           0x00000010    1382         add r8, r2
           0x00000012    7f15         ret
```
Yay! it works.. and the mandatory oneliner too!

```
r2 -nqamycpu -cwoR -cpd' 10' -
```

## Writing the r_anal plugin

Then it's time for analysis plugin

## Static plugins in core

Pushing a new architecture into the main branch of r2 requires to modify several files in order to make it fit into the way the rest of plugins are built.

List of affected files:

* `plugins.def.cfg` : add the asm.mycpu plugin name string in there
* `libr/asm/p/mycpu.mk` : build instructions
* `libr/asm/p/asm_mycpu.c` : implementation
* `libr/include/r_asm.h` : add the struct definition in there

Check out how the NIOS II cpu was implemented by reading those commits:

Implement RAsm plugin:
https://github.com/radare/radare2/commit/933dc0ef6ddfe44c88bbb261165bf8f8b531476b

Implement RAnal plugin:
https://github.com/radare/radare2/commit/ad430f0d52fbe933e0830c49ee607e9b0e4ac8f2

## Write a disassembler plugin with another programming language

### Python

At first, to be able to write a plugins in Python for radare2 you need to install
r2lang plugin. If you're going to use Python 2, then use `r2pm -i lang-python2`, otherwise (and recommended) - install the Python 3 version: `r2pm -i lang-python3`.
Note - in the following examples there are missing functions of the actual decoding
for the sake of readability!

For this you need to do this:
1. `import r2lang` and `from r2lang import R` (for constants)
2. Make a function with 2 subfunctions - `assemble` and `disassemble` and returning plugin structure - for RAsm plugin
```python
def mycpu(a):
    def assemble(s):
        return [1, 2, 3, 4]

    def disassemble(buf):
        try:
            opcode = get_opcode(buf)
            opstr = optbl[opcode][1]
            return [4, opstr]
        except:
            return [4, "unknown"]
```
3. This structure should contain a pointers to these 2 functions - `assemble` and `disassemble`

```python
    return {
            "name" : "mycpu",
            "arch" : "mycpu",
            "bits" : 32,
            "endian" : "little",
            "license" : "GPL",
            "desc" : "MYCPU disasm",
            "assemble" : assemble,
            "disassemble" : disassemble,
    }
```
4. Make a function with 2 subfunctions - `set_reg_profile` and `op` and returning plugin structure - for RAnal plugin

```python
def mycpu_anal(a):
       def set_reg_profile():
        profile = "=PC	pc\n" + \
		"=SP	sp\n" + \
		"gpr	r0	.32	0	0\n" + \
		"gpr	r1	.32	4	0\n" + \
		"gpr	r2	.32	8	0\n" + \
		"gpr	r3	.32	12	0\n" + \
		"gpr	r4	.32	16	0\n" + \
		"gpr	r5	.32	20	0\n" + \
		"gpr	sp	.32	24	0\n" + \
		"gpr	pc	.32	28	0\n"
        return profile

    def op(addr, buf):
		analop = {
            "type" : R.R_ANAL_OP_TYPE_NULL,
            "cycles" : 0,
            "stackop" : 0,
            "stackptr" : 0,
			"ptr" : -1,
            "jump" : -1,
            "addr" : 0,
            "eob" : False,
            "esil" : "",
        }
        try:
            opcode = get_opcode(buf)
            esilstr = optbl[opcode][2]
            if optbl[opcode][0] == "J": # it's jump
                analop["type"] = R.R_ANAL_OP_TYPE_JMP
                analop["jump"] = decode_jump(opcode, j_mask)
                esilstr = jump_esil(esilstr, opcode, j_mask)

        except:
            result = analop
		# Don't forget to return proper instruction size!
        return [4, result]

```
5. This structure should contain a pointers to these 2 functions - `set_reg_profile` and `op`

```python
    return {
            "name" : "mycpu",
            "arch" : "mycpu",
            "bits" : 32,
            "license" : "GPL",
            "desc" : "MYCPU anal",
            "esil" : 1,
            "set_reg_profile" : set_reg_profile,
            "op" : op,
    }
```
6. Then register those using `r2lang.plugin("asm")` and `r2lang.plugin("anal")` respectively

```python
print("Registering MYCPU disasm plugin...")
print(r2lang.plugin("asm", mycpu))
print("Registering MYCPU analysis plugin...")
print(r2lang.plugin("anal", mycpu_anal))
```

You can combine everything in one file and load it using `-i` option:
```
r2 -I mycpu.py some_file.bin
```
Or you can load it from the r2 shell: `#!python mycpu.py`

See also:

* [Python](https://github.com/radare/radare2-bindings/blob/master/libr/lang/p/test-py-asm.py)
* [Javascript](https://github.com/radare/radare2-bindings/blob/master/libr/lang/p/dukasm.js)

More examples to come...

## Write a debugger plugin

* Adding the debugger registers profile into the shlr/gdb/src/core.c
* Adding the registers profile and architecture support in the libr/debug/p/debug_native.c and libr/debug/p/debug_gdb.c
* Add the code to apply the profiles into the function `r_debug_gdb_attach(RDebug *dbg, int pid)`

If you want to add support for the gdb, you can see the register profile in the active gdb session using command `maint print registers`.

## More to come..

* Related article: http://radare.today/posts/extending-r2-with-new-plugins/

Some commits related to "Implementing a new architecture"

* Extensa: https://github.com/radare/radare2/commit/6f1655c49160fe9a287020537afe0fb8049085d7
* Malbolge: https://github.com/radare/radare2/pull/579
* 6502: https://github.com/radare/radare2/pull/656
* h8300: https://github.com/radare/radare2/pull/664
* GBA: https://github.com/radare/radare2/pull/702
* CR16: https://github.com/radare/radare2/pull/721/ && https://github.com/radare/radare2/pull/726
* XCore: https://github.com/radare/radare2/commit/bb16d1737ca5a471142f16ccfa7d444d2713a54d
* Sharp LH5801: https://github.com/neuschaefer/radare2/commit/f4993cca634161ce6f82a64596fce45fe6b818e7
* MSP430: https://github.com/radare/radare2/pull/1426
* HP PA-RISC: https://github.com/radare/radare2/commit/f8384feb6ba019b91229adb8fd6e0314b0656f7b
* V810: https://github.com/radare/radare2/pull/2899
* TMS320: https://github.com/radare/radare2/pull/596

## Implementing a new pseudo architecture

Example:

* **Z80**: https://github.com/radare/radare2/commit/8ff6a92f65331cf8ad74cd0f44a60c258b137a06


## Implementing a new analysis plugin

```makefile
NAME=anal_snes
R2_PLUGIN_PATH=$(shell r2 -hh|grep LIBR_PLUGINS|awk '{print $$2}')
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
_d__  _8_16      snes        LGPL3   SuperNES CPU
```

**snes_op_table**.h: https://github.com/radare/radare2/blob/master/libr/asm/arch/snes/snes_op_table.h

Example:

* **6502**: https://github.com/radare/radare2/commit/64636e9505f9ca8b408958d3c01ac8e3ce254a9b
* **SNES**: https://github.com/radare/radare2/commit/60d6e5a1b9d244c7085b22ae8985d00027624b49

## Implementing a new format

### To enable virtual addressing

In `info` add `et->has_va = 1;` and `ptr->srwx` with the `R_BIN_SCN_MAP;` attribute

### Create a folder with file format name in libr/bin/format

**Makefile:**

```Makefile
NAME=bin_nes
R2_PLUGIN_PATH=$(shell r2 -hh|grep LIBR_PLUGINS|awk '{print $$2}')
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

### Implementing new format plugin in Python

Note - in the following examples there are missing functions of the actual decoding
for the sake of readability!

For this you need to do this:
1. `import r2lang`
2. Make a function with  subfunctions:
   - `load`
   - `load_bytes`
   - `destroy`
   - `check_bytes`
   - `baddr`
   - `entries`
   - `sections`
   - `imports`
   - `relocs`
   - `binsym`
   - `info`

   and returning plugin structure - for RAsm plugin
```python
def le_format(a):
    def load(binf):
        return [0]

    def check_bytes(buf):
        try:
			if buf[0] == 77 and buf[1] == 90:
                lx_off, = struct.unpack("<I", buf[0x3c:0x40])
                if buf[lx_off] == 76 and buf[lx_off+1] == 88:
                    return [1]
            return [0]
        except:
            return [0]
```
and so on. Please be sure of the parameters for each function and format of returns.
Note, that functions `entries`, `sections`, `imports`, `relocs` returns a list of special
formed dictionaries - each with a different type.
Other functions return just a list of numerical values, even if single element one.
There is a special function, which returns information about the file - `info`:
```python
    def info(binf):
        return [{
                "type" : "le",
                "bclass" : "le",
                "rclass" : "le",
                "os" : "OS/2",
                "subsystem" : "CLI",
                "machine" : "IBM",
                "arch" : "x86",
                "has_va" : 0,
                "bits" : 32,
                "big_endian" : 0,
                "dbg_info" : 0,
                }]
```

3. This structure should contain a pointers to the most important functions like
`check_bytes`, `load` and `load_bytes`, `entries`, `relocs`, `imports`, etc.

```python
    return {
            "name" : "le",
            "desc" : "OS/2 LE/LX format",
            "license" : "GPL",
            "load" : load,
            "load_bytes" : load_bytes,
            "destroy" : destroy,
            "check_bytes" : check_bytes,
            "baddr" : baddr,
            "entries" : entries,
            "sections" : sections,
            "imports" : imports,
            "symbols" : symbols,
            "relocs" : relocs,
            "binsym" : binsym,
            "info" : info,
    }
```
4. Then you need to register it as a file format plugin:

```python
print("Registering OS/2 LE/LX plugin...")
print(r2lang.plugin("bin", le_format))
```

EXT_SO=$(shell r2 -H R2_LIBEXT)
R2_USER_PLUGINS=$(shell r2 -H R2_USER_PLUGINS)
R2_CFLAGS=$(shell pkg-config --cflags r_core)
R2_LDFLAGS=$(shell pkg-config --libs r_core)

R2BOOK_HOME=$(shell cd ../src; pwd)

CFLAGS+=-DR2BOOK_HOME=\"$(R2BOOK_HOME)\"
CFLAGS+=-g

# CFLAGS+=-fsanitize=address

all:
	$(CC) $(CFLAGS) $(R2_CFLAGS) $(R2_LDFLAGS) core_r2book.c -shared -o core_r2book.$(EXT_SO)
	$(MAKE) install

install:
	mkdir -p $(R2_USER_PLUGINS)
	cp *.$(EXT_SO) $(R2_USER_PLUGINS)

uninstall:
	rm -f $(R2_USER_PLUGINS)/core_r2book.$(EXT_SO)

all: pdf epub

.PHONY: all lint lint-fix epub pdf gmi texi info one one-online

# CHAPTERS=$(shell find src -iname *.md)
CHAPTERS=$(shell grep -Eoi '\([^\)]+' src/SUMMARY.md | sed -e 's,^.,src/,')

# Global pandoc arguments
PANDOC_OPTIONS+=--from markdown+rebase_relative_paths
PANDOC_OPTIONS+=--metadata-file=pandoc/metadata.yaml
PANDOC_OPTIONS+=--lua-filter=pandoc/filter.lua

# PDF specific pandoc arguments
PANDOC_PDF_OPTIONS+=--pdf-engine=xelatex
PANDOC_PDF_OPTIONS+=-H pandoc/header.tex --listings
## Add a cover and hide title text page
PANDOC_PDF_OPTIONS+=-B pandoc/cover.tex -V title:

# MD specific pandoc arguments
PANDOC_MD_OPTIONS+=--to markdown+backtick_code_blocks
PANDOC_MD_OPTIONS+=--wrap=preserve

# MD external paths
GIT_COMMIT:=$(shell git rev-parse HEAD)
URL_PREFIX:=https://github.com/radareorg/radare2-book
URL_MD_PREFIX:=$(URL_PREFIX)/blob/$(GIT_COMMIT)/
URL_IMAGE_PREFIX:=$(URL_PREFIX)/raw/$(GIT_COMMIT)/

pdf:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) $(PANDOC_PDF_OPTIONS) -o r2book.pdf

epub:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) -o r2book.epub

texi:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) -o r2book.texi

info: texi
	rm -f r2book.info r2book.info.gz
	makeinfo --force --no-split r2book.texi
	gzip -9n r2book.info

one:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) $(PANDOC_MD_OPTIONS) -o r2book.md

one-online: one
	sed -i -E \
		-e 's,\((src/[0-9A-Za-z./_-]+.md)\),($(URL_MD_PREFIX)\1),g' \
		-e 's,\(((src|images)/[0-9A-Za-z./_-]+.(png|jpg))\),($(URL_IMAGE_PREFIX)\1),g' \
		r2book.md

GOPATH?=$(HOME)/go
GOBIN?=$(GOPATH)/bin
MD2GMI?=$(GOBIN)/md2gmi

gmi:
	@test -x $(MD2GMI) || (echo "ERROR: Missing md2gmi.\nTo install run: go install github.com/n0x1m/md2gmi@latest"; false)
	rm -rf gmi
	mkdir -p gmi
	for a in $(shell find src -type d) ; do b=`echo $$a |sed -e 's,src/,gmi/,'`; mkdir -p $$b ; done
	for a in $(shell find src -type f -name \*.md) ; do \
		b=`echo $$a | sed -e 's,src/,gmi/,' -e 's,\.md,\.gmi,'` ; \
		$(MD2GMI) -i $$a -o $$b; \
		sed -e 's,\.md,\.gmi,' -i $$b; \
	done
	grep -v 134 < gmi/SUMMARY.gmi | grep -v '^-' > .a.a
	mv .a.a gmi/SUMMARY.gmi
	ln -v gmi/SUMMARY.gmi gmi/index.gmi
	tar -czf r2book-gmi.tar.gz -C gmi .

# Linting
MDLINT_GLOBS='src/**/*.md'
MDLINT_CONFIG=src/.markdownlint.jsonc
node_modules/.bin/markdownlint-cli2:
	npm install --no-save markdownlint-cli2

lint: node_modules/.bin/markdownlint-cli2
	sys/lintrefs.sh
	node_modules/.bin/markdownlint-cli2 $(MDLINT_GLOBS) --config $(MDLINT_CONFIG)

lint-fix: node_modules/.bin/markdownlint-cli2
	node_modules/.bin/markdownlint-cli2 $(MDLINT_GLOBS) --config $(MDLINT_CONFIG) --fix
	sys/lintrefs.sh

lint-one: node_modules/.bin/markdownlint-cli2 lint one
	node_modules/.bin/markdownlint-cli2 r2book.md --config .one.markdownlint-cli2.jsonc

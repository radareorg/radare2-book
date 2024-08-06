all: pdf epub

.PHONY: epub pdf pdf_old gmi texi info

pdf_old: node_modules
	npm run build
	npm run pdf

node_modules:
	mkdir -p node_modules
	npm i

# CHAPTERS=$(shell find src -iname *.md)
CHAPTERS=$(shell grep -Eoi '\([^\)]+' src/SUMMARY.md | sed -e 's,^.,src/,')

# Global pandoc arguments
PANDOC_OPTIONS+=-f markdown+rebase_relative_paths
PANDOC_OPTIONS+=--metadata-file=metadata.yaml
PANDOC_OPTIONS+=--lua-filter=pandoc-filter.lua

# PDF specific pandoc arguments
PANDOC_PDF_OPTIONS+=--pdf-engine=xelatex
PANDOC_PDF_OPTIONS+=-H header.tex --listings
## Add a cover and hide title text page
PANDOC_PDF_OPTIONS+=-B cover.tex -V title:

pdf:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) $(PANDOC_PDF_OPTIONS) -o r2book.pdf

epub:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) -o r2book.epub

texi:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) -o r2book.texi

one r2book.md:
	sh one.sh > r2book.md

info: texi
	rm -f r2book.info r2book.info.gz
	makeinfo --force --no-split r2book.texi
	gzip -9n r2book.info

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

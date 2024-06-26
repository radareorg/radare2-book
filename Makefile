all: pdf epub

.PHONY: epub pdf pdf_old gmi

pdf_old: node_modules
	npm run build
	npm run pdf

node_modules:
	mkdir -p node_modules
	npm i

# CHAPTERS=$(shell find src -iname *.md)
CHAPTERS=$(shell grep -Eoi '\([^\)]+' src/SUMMARY.md | sed -e 's,^.,src/,')

PANDOC_OPTIONS+=-f markdown+rebase_relative_paths
PANDOC_OPTIONS+=--metadata-file=metadata.yaml
PANDOC_OPTIONS+=--toc-depth=5 --toc
PANDOC_OPTIONS+=-V papersize:a5
PANDOC_OPTIONS+=-V geometry:margin=1.5cm

# PDF: Add cover and hide title text page
PANDOC_PDF_OPTIONS+=-B cover.tex -V title:
PANDOC_PDF_OPTIONS+=--pdf-engine=xelatex

PANDOC_EPUB_OPTIONS+=--epub-cover-image=cover.jpg

pdf:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) $(PANDOC_PDF_OPTIONS) -o r2book.pdf

epub:
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) $(PANDOC_EPUB_OPTIONS) -o r2book.epub

MD2GMI=md2gmi/md2gmi

md2gmi:
	git clone https://github.com/n0x1m/md2gmi

$(MD2GMI): md2gmi
	cd md2gmi && go build .

gmi: $(MD2GMI)
	rm -rf gmi
	mkdir -p gmi
	for a in $(shell find src -type d) ; do b=`echo $$a |sed -e 's,src/,gmi/,'`; mkdir -p $$b ; done
	for a in $(shell find src | grep md$$) ; do b=`echo $$a |sed -e 's,src/,gmi/,' -e 's,md$$,gmi,'` ; $(MD2GMI) -o $$b < $$a; done
	cp -f gmi/intro.gmi gmi/index.gmi

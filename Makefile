all: pdf epub

pdf_old: node_modules
	npm run build
	npm run pdf

node_modules:
	mkdir -p node_modules
	npm i

# CHAPTERS=$(shell find src -iname *.md)
CHAPTERS=$(shell grep -Eoi '\([^\)]+' src/SUMMARY.md | sed -e 's,^.,src/,')
PANDOC_PDF_OPTIONS+=-V papersize:a3
PANDOC_PDF_OPTIONS+=-V geometry:margin=1.5cm
PANDOC_PDF_OPTIONS+=--pdf-engine=xelatex
PANDOC_PDF_OPTIONS+=--resource-path=img
PANDOC_PDF_OPTIONS+=--toc-depth=5 --toc
PANDOC_PDF_OPTIONS+=-B src/COVER.md
# PANDOC_OPTIONS+=--template md.template
PANDOC_EPUB_OPTIONS+=-V papersize:a3
PANDOC_EPUB_OPTIONS+=-V geometry:margin=1.5cm
PANDOC_EPUB_OPTIONS+=--resource-path=img
PANDOC_EPUB_OPTIONS+=--toc-depth=5 --toc
PANDOC_EPUB_OPTIONS+=--epub-cover-image=cover.jpg

pdf:
	# Cant add images because of this bug: https://github.com/jgm/pandoc/issues/3752
	# pandoc src/**/*.md --verbose --pdf-engine=xelatex -o r2book.pdf
	# pandoc  src/**/*.md --pdf-engine=xelatex -o r2book.pdf
	rm -rf img ; mkdir -p img ; for a in $(shell find src | grep png$$) ; do cp $$a img ; done
	cp cover.jpg img
	pandoc $(CHAPTERS) $(PANDOC_OPTIONS) -o r2book.pdf
	rm -rf img

epub:
	# Cant add images because of this bug: https://github.com/jgm/pandoc/issues/3752
	# pandoc src/**/*.md --verbose --pdf-engine=xelatex -o r2book.pdf
	# pandoc  src/**/*.md --pdf-engine=xelatex -o r2book.pdf
	rm -rf img ; mkdir -p img ; for a in $(shell find src | grep png$$) ; do cp $$a img ; done
	cp cover.jpg img
	pandoc $(CHAPTERS) $(PANDOC_EPUB_OPTIONS) -o r2book.epub
	rm -rf img

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

.PHONY: gmi

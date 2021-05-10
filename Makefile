all:

pdf_old: node_modules
	npm run build
	npm run pdf

node_modules:
	mkdir -p node_modules
	npm i

pdf:
	# Cant add images because of this bug: https://github.com/jgm/pandoc/issues/3752
	# pandoc src/**/*.md --verbose --pdf-engine=xelatex -o r2book.pdf
	pandoc  src/**/*.md --pdf-engine=xelatex -o r2book.pdf

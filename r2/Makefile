R2PNG=node r2png/index.js
R2SNIP=node r2snip.js
D=../img-white/

all: r2png/node_modules
	$(R2SNIP) `find .. | grep -v _r2book | grep snippets$$` 

r2png/node_modules:
	mkdir -p r2png/node_modules
	cd r2png ; npm i

test:
	mkdir -p $(D)/configuration
	#$(R2PNG) bins/ls 'e??~color' > $(D)/configuration/e--color.png
	$(R2PNG) - 'hello world' > a.png
	#$(R2PNG) bins/ls 'pd 1 @ 0x00404894' > output.png



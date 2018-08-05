R2PNG=node r2png/index.js
R2SNIP=node r2snip.js
D=../img-white/

all:
	$(R2SNIP) `find ../| grep snippets$$` 
	mkdir -p $(D)/configuration
	#$(R2PNG) bins/ls 'e??~color' > $(D)/configuration/e--color.png
	$(R2PNG) - 'hello world' > a.png
	#$(R2PNG) bins/ls 'pd 1 @ 0x00404894' > output.png



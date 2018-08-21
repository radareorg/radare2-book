all: node_modules
	npm run build
#	npm run pdf

pdf:
	npm run pdf

node_modules:
	mkdir -p node_modules
	npm i

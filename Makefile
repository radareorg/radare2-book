all: node_modules
	npm run build

node_modules:
	mkdir -p node_modules
	npm i

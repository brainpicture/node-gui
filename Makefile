all:
	node-waf configure build
tests:
	node ./test.js
install:
	cp ./build/default/node-gtk.node ~/.node_libraries/node-gtk.node
clean:
	rm -rf ./build

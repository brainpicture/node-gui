all:
	node-waf configure build
tests:
	node ./test.js
install:
	cp ./build/default/node-gtk.node /usr/local/lib/node/libraries/node-gtk.node
clean:
	rm -rf ./build

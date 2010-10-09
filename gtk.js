var gtk=require('./build/default/node-gtk');
var sys=require('sys');

exports.main = function() {
	if (!gtk.main()) setTimeout(function() {
		exports.main();
	},20);
};

exports.window = function(params) {
	var window = gtk.window(params);
	if (params.title != null) {
    window.setTitle(params.title);
  }
	if (params.width != null || params.height != null) {
    window.setSize(params.width, params.height);
  }
	if (params.dimensions != null) {
    window.setFrameDimensions(params.dimensions.left, params.dimensions.top, params.dimensions.right, params.dimensions.bottom);
  }
	if (params.position != null) {
    window.setPosition(params.position);
  }
	if (params.opacity != null) {
    window.setOpacity(params.opacity);
  }
	if (params.resizable != null) {
    window.setResizable(params.resizable);
  }
	if (params.onClose != null) {
    window.onClose(params.onClose);
  }

	window.onShow = exports.main;

	window.container = exports.v({items: params.items});
	window.add(window.container);
	window.add = function(obj) {
		window.container.add(obj);
	};
	return window;
};

exports.button = function(params) {
	var button = gtk.button();

	if (params.title != null) button.setTitle(params.title);
	if (params.onClick != null) button.onClick(params.onClick);

	return button;
};

exports.entry = function(params) {
	var entry = gtk.entry();
	if (params.text != null) entry.setText(params.text);
	return entry;
};

exports.hbox = exports.h = function(params) {
	var hbox = gtk.hbox();
  if (typeof(params)=='object' && (params instanceof Array)) params = {items: params};
	if (params.items) for (i in params.items) {
		hbox.add(params.items[i]);
	}
	return hbox;
};

exports.vbox = exports.v = function(params) {
	var vbox = gtk.vbox();
  if (typeof(params)=='object' && (params instanceof Array)) params = {items: params};
	if (params.items) for (i in params.items) {
		vbox.add(params.items[i]);
	}
	return vbox;
};

exports.alert = function(title) {
	gtk.alert(title);
};

var gtk=require('./build/default/node-gtk');
var sys=require('sys');

exports.main = function() {
	if (!gtk.main()) process.nextTick(function() {
		exports.main();
	});
};

exports.window = function(params) {
	var window = gtk.window(params);
	if (params.title != null) window.set_title(params.title);
	if (params.width != null || params.height != null) window.set_size(params.width, params.height);
	if (params.dimensions != null) window.set_frame_dimensions(params.dimensions.left, params.dimensions.top, params.dimensions.right, params.dimensions.bottom);
	if (params.position != null) window.set_position(params.position);
	if (params.opacity != null) window.set_opacity(params.opacity);
	if (params.resizable != null) window.set_resizable(params.resizable);
	if (params.on_close != null) window.on_close(params.on_close);
	window.on_show = exports.main;

	window.container = exports.v({items: params.items});
	window.add(window.container);
	window.add = function(obj) {
		window.container.add(obj);
	};
	return window;
};

exports.button = function(params) {
	var button = gtk.button();
	if (params.title != null) button.set_title(params.title);
	if (params.on_click != null) button.on_click(params.on_click);
	return button;
};

exports.entry = function(params) {
	var entry = gtk.entry();
	if (params.text != null) entry.set_text(params.text);
	return entry;
};

exports.HBox = exports.h = function(params) {
	var hbox = gtk.HBox();
  if (typeof(params)=='object' && (params instanceof Array)) params = {items: params};
	if (params.items) for (i in params.items) {
		hbox.add(params.items[i]);
	}
	return hbox;
};

exports.VBox = exports.v = function(params) {
	var vbox = gtk.VBox();
  if (typeof(params)=='object' && (params instanceof Array)) params = {items: params};
	if (params.items) for (i in params.items) {
		vbox.add(params.items[i]);
	}
	return vbox;
};

exports.alert = function(title) {
	gtk.alert(title);
};

var gtk=require('./build/default/gtk');
var sys=require('sys');

exports.main = function() {
	if (!gtk.main()) process.nextTick(function() {
		exports.main();
	});
};

exports.window = function(params) {
	var window = gtk.window(params);
	if (params.title != null) window.setTitle(params.title);
	if (params.width != null || params.height != null) window.setSize(params.width, params.height);
	if (params.onClose != null) window.onClose(params.onClose);
	window.onShow = exports.main;
	
	window.container = exports.v({items: params.items});
	window.add(window.container);
	window.add = function(obj) {
		window.container.add(obj);
	}
	return window;
}

exports.button = function(params) {
	var button = gtk.button();
	if (params.title != null) button.setTitle(params.title);
	if (params.onClick != null) button.onClick(params.onClick);
	return button;
}

exports.HBox = exports.h = function(params) {
	var hbox = gtk.HBox();
	if (typeof(params)=='object' && (params instanceof Array)) params = {items: params}
	if (params.items) for (i in params.items) {
		hbox.add(params.items[i]);
	}
	return hbox;
}

exports.VBox = exports.v = function(params) {
	var vbox = gtk.VBox();
	if (typeof(params)=='object' && (params instanceof Array)) params = {items: params}
	if (params.items) for (i in params.items) {
		vbox.add(params.items[i]);
	}
	return vbox;
}

exports.alert = function(title) {
	gtk.alert(title);
}

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
	return window;
}

exports.button = function(params) {
	var button = gtk.button();
	if (params.title != null) button.setTitle(params.title);
	if (params.onClick != null) button.onClick(params.onClick);
	return button;
}

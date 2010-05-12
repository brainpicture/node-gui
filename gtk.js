var gtk=require('./build/default/gtk');
var sys=require('sys');

exports.window = function(params) {
	var window = gtk.window(params);
	if (params.title != null) window.setTitle(params.title);
	if (params.onClose != null) window.onClose = params.onClose;
	if (params.width != null || params.height != null) window.setSize(params.width, params.height);
	return window;
}

exports.button = function(params) {
	var button = gtk.button();
	if (params.title != null) button.setTitle(params.title);
	return button;
}

exports.main = function() {
	process.nextTick(function() {
		gtk.main();
		exports.main();
	});
}

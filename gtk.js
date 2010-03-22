var gtk=require('./build/default/gtk');

exports.window = function(params) {
	var window = gtk.window(params);
	if (params.title != null) window.setTitle(params.title);
	if (params.onClose != null) window.onClose = params.onClose;
	return window;
}

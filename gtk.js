var gtk=require('./build/default/gtk');

exports.window = function(params) {
	var window = gtk.window(params);
	if (params.title != null) window.setTitle(params.title);
	return window;
}

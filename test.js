var gui=require('./gtk');
var sys=require('sys');

sys.puts('test 1');
var wnd = gui.window({
	title:"test Application",
	width: 300,
	height: 200,
	onClose: function() {
		sys.puts('wow');
	}
}).show();
sys.puts('test 2');

var gui=require('./gtk');
var sys=require('sys');

var wnd = gui.window({
	title:"test Application",
	width: 300,
	height: 200	
}).show();
sys.puts(wnd);

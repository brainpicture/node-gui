var gui=require('./gtk');
var sys=require('sys');

sys.puts('test 1');

var wnd = gui.window({
	title: 'Тестовое приложение',
	width: 600,
	height: 200,
	onClose: function() {
		sys.puts('wow11 WOWOWOWOWOWO');
	}
}).show();

wnd.add(gui.button({title: 'тест тест'}));

gui.main();
sys.puts('test 2');

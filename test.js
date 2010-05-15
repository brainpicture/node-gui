var gui=require('./gtk');
var sys=require('sys');

var wnd = gui.window({
	title: 'Тестовое приложение',
	width: 600,
	height: 200,
	onClose: function() {
		sys.puts('wow11 WOWOWOWOWOWO');
	}
}).show();

wnd.add(gui.button({
	title: 'тест тест',
	onClick: function() {
		sys.puts('wow wow pressed тест');
	}
}));

gui.main();
//setInterval(function() {
//	sys.puts('working');
//},1000);

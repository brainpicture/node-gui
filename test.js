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
		var wnd2 = gui.window({
			title: 'Тестовое приложение2',
			width: 600,
			height: 200,
			onClose: function() {
				sys.puts('wow11 WOWOWOWOWOWO');
			}
		}).show();
	}
}));

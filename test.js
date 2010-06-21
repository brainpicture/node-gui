var gui=require('./gtk');
var sys=require('sys');

var wnd = gui.window({
	title: 'Тестовое приложение',
	width: 600,
	height: 400,
	items: [
		gui.button({
			title: 'шапка',
		}),
		gui.h([
			gui.button({
				title: 'лево',
			}),
			gui.button({
				title: 'право',
				onClick: function() {
					gui.alert('wow');
				}
			})
		]),
		gui.button({
			title: 'футер',
		})
	],
	onClose: function() {
		gui.alert('Good bye!');
	}
}).show();

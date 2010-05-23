## Some basic GTK functions bindings
It's just an experement, but if you're interested, you can fork it!

Now works:
	var gui=require('./gtk');

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
			gui.alert('Good bie!');
		}
	}).show();





Design, I suggest:
	var gui=require('./gtk');

	var wnd=gui.window({
		name: "test Application",
		width: 300,
		height: 200,
		items: [
			gui.H([
				gui.label({
					text: 'Enter your name:'
				}),
				gui.input({
					value: 'Here'
				})
			]),
			gui.V([
				gui.button({
					text: 'Save',
					onClick: function() {
						save(wnd.items.H[0].input[0].value);
					}
				}),
				gui.button({
					text: 'Cancel',
					onClick: function() {
						wnd.close();
					}
				})
			])
		]
	}).show();

TODO:
-----------

GtkWindow
* All Getters
* Accelorator Groups (keyboard combos)
* Focus (need to pass widgets around)
* Pagers
* Hints
* Gravity
* Make GtkWidget bindings and subclass it for shared methods
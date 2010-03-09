## Some basic GTK functions bindings
It's just an experement, but if you're interested, you can fork it!

Now works:
	var gui=require('./gtk');

	gui.window({
		name: "test Application",
		width: 300,
		height: 200	
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

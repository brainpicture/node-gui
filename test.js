var gtk = require('./build/default/gtk');

var window = new gtk.Window();

var button = new gtk.Button();
button.setLabel('Test Button');

button.on('clicked', function () {
  console.log(gtk.depth());
  console.log('clicked');
});

window.add(button);

window.setTitle('Node');
window.setResizable(true);
window.setDefaultSize();
window.setPosition();
window.setOpacity(0.9);

var dialog = new gtk.MessageDialog(window, gtk.DIALOG_DESTROY_WITH_PARENT,
                     gtk.MESSAGE_INFO, gtk.BUTTONS_YES_NO, "Testing");

window.on('destroy', function () {
  console.log(window.getOpacity());
  console.log(window.getPosition());
  console.log(window.getSize());
  console.log(window.getResizable(true));
  console.log(window.getTitle());
  console.log('OMG');
  gtk.quit();
});

window.show();
dialog.show();

gtk.run();

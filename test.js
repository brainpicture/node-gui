var gtk = require('./build/default/gtk');

var loop = new gtk.Loop();

var window = new gtk.Window();

var button = new gtk.Button();
button.setLabel('Test Button');

button.on('clicked', function () {
  console.log('clicked');
  var dialog = new gtk.MessageDialog(window, gtk.DIALOG_DESTROY_WITH_PARENT,
                       gtk.MESSAGE_INFO, gtk.BUTTONS_YES_NO, "Testing");
  dialog.show();
});

window.add(button);

window.setTitle('Node');
window.setResizable(true);
window.setDefaultSize();

window.on('destroy', function () {
  console.log(window.getOpacity());
  console.log(window.getPosition());
  console.log(window.getSize());
  console.log(window.getResizable(true));
  console.log(window.getTitle());
  console.log('OMG');
  loop.quit();
});

window.show();

loop.run();

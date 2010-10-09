var gtk = require('./build/default/gtk');

console.log(gtk);

exports.main = function() {
  if (!gtk.main()) setTimeout(function() {
    exports.main();
  },20);
};

var window = new gtk.Window();

var button = new gtk.Button();
button.setLabel('Test Button');

button.onClick = function () {
  console.log('clicked');
};

window.add(button);

window.onShow = exports.main;
window.setTitle('Node');
window.setResizable(true);
window.setDefaultSize();
window.setPosition();
window.setOpacity(0.9);

var dialog = new gtk.MessageDialog(window, gtk.DIALOG_DESTROY_WITH_PARENT,
                     gtk.MESSAGE_INFO, gtk.BUTTONS_YES_NO, "Testing");

window.onDestroy = function () {
  console.log(window.getOpacity());
  console.log(window.getPosition());
  console.log(window.getSize());
  console.log(window.getResizable(true));
  console.log(window.getTitle());
  console.log('OMG');
};

window.show();
dialog.run();
dialog.destroy();

var gtk = require('./build/default/node-gtk');

exports.main = function() {
  if (!gtk.main()) setTimeout(function() {
    exports.main();
  },20);
};

var window = new gtk.Window();

window.onShow = exports.main;
window.setTitle('Node');
window.setResizable(true);
window.setDefaultSize();
window.setPosition();
window.setOpacity(0.7);

window.onDestroy = function () {
  console.log(window.getOpacity());
  console.log(window.getPosition());
  console.log(window.getSize());
  console.log(window.getResizable(true));
  console.log(window.getTitle());
  console.log('OMG');
};

window.show();

var gtk          = require('./build/default/node-gtk'),
    EventEmitter = require('events').EventEmitter;

exports.main = function() {
  if (!gtk.main()) setTimeout(function() {
    exports.main();
  },20);
};

var proto;

var Window = function (params) {
  var key,
      self = this,
      keys = Object.keys(params);

  this.widget = gtk.window();

  this.widget.onShow = function () {
    exports.main();
    self.emit('show');
  };
  this.widget.onClose = function () {
    console.log('close');
    self.emit('close');
  };

  for (var i = 0, il = keys.length; i < il; i++) {
    key = keys[i];

    if (this.hasOwnProperty(key)) this[key] = params[key];
  }
};

// Export Window.
exports.Window = Window;

Window.prototype = Object.create(EventEmitter.prototype);

proto = Window.prototype;

// Window#title
Object.defineProperty(proto, 'title', {
  enumerable: true,
  get: function () {
    return this.widget.getTitle();
  },
  set: function (value) {
    this.widget.setTitle(value);
    return this;
  }
});

// Window#resizable
Object.defineProperty(proto, 'resizable', {
  enumerable: true,
  get: function () {
    return this.widget.getResizable();
  },
  set: function (value) {
    this.widget.setResizable(!!value);
    return this;
  }
});

proto.show = function () {
  this.widget.show();
  return this;
};

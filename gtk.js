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

  this.widget.container = exports.v({items: params.items});
  this.widget.add(this.widget.container);
  this.widget.add = function(obj) {
    self.widget.container.add(obj);
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

exports.window = function(params) {
  var window = gtk.window();
  params || (params = {});
  if (params.width != null || params.height != null) {
    window.setSize(params.width, params.height);
  }
  if (params.dimensions != null) {
    window.setFrameDimensions(params.dimensions.left, params.dimensions.top, params.dimensions.right, params.dimensions.bottom);
  }
  if (params.position != null) {
    window.setPosition(params.position);
  }
  if (params.opacity != null) {
    window.setOpacity(params.opacity);
  }

  window.onShow = exports.main;

  window.container = exports.v({items: params.items});
  window.add(window.container);
  window.add = function(obj) {
    window.container.add(obj);
  };
  return window;
};

exports.button = function(params) {
  var button = gtk.button();

  if (params.title != null) button.setTitle(params.title);
  if (params.onClick != null) button.onClick(params.onClick);

  return button;
};

exports.entry = function(params) {
  var entry = gtk.entry();
  if (params.text != null) entry.setText(params.text);
  return entry;
};

exports.hbox = exports.h = function(params) {
  var hbox = gtk.hbox();
  if (typeof(params)=='object' && (params instanceof Array)) params = {items: params};
  if (params.items) for (i in params.items) {
    hbox.add(params.items[i]);
  }
  return hbox;
};

exports.vbox = exports.v = function(params) {
  var vbox = gtk.vbox();
  if (typeof(params)=='object' && (params instanceof Array)) params = {items: params};
  if (params.items) for (i in params.items) {
    vbox.add(params.items[i]);
  }
  return vbox;
};

exports.alert = function(title) {
  gtk.alert(title);
};

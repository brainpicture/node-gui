#ifndef NGTK_WIDGET_H_
#define NGTK_WIDGET_H_

#include <v8.h>
#include <node.h>

#include <gtk/gtk.h>          // GtkWidget
#include <node_object_wrap.h> // node::ObjectWrap

#include "ngtk.h"

namespace ngtk {

class Widget : public node::ObjectWrap {
public:
  // For getting the underlying GtkWidget
  static void Initialize (v8::Handle<v8::FunctionTemplate> constructor_template) {
    v8::HandleScope scope;

    NGTK_SET_PROTOTYPE_METHOD(constructor_template, "on",      Widget::On);
    NGTK_SET_PROTOTYPE_METHOD(constructor_template, "show",    Widget::Show);
    NGTK_SET_PROTOTYPE_METHOD(constructor_template, "destroy", Widget::Destroy);
  }

  // For getting the underlying GtkWidget
  static inline GtkWidget* Gtk (v8::Handle<v8::Object> obj) {
    v8::HandleScope scope;

    return node::ObjectWrap::Unwrap<Widget>(obj)->widget_;
  }

  GtkWidget *widget_;

private:
  // Add signal handler.
  static inline v8::Handle<v8::Value> On (const v8::Arguments &args) {
    v8::HandleScope scope;

    if (args[0]->IsString() && args[1]->IsFunction()) {
      GtkWidget *widget = node::ObjectWrap::Unwrap<Widget>(args.This())->widget_;

      v8::Persistent<v8::Function> *callback = new v8::Persistent<v8::Function>();
      *callback = v8::Persistent<v8::Function>::New(v8::Handle<v8::Function>::Cast(args[1]));

      g_signal_connect(G_OBJECT(widget), *v8::String::Utf8Value(args[0]->ToString()),
          G_CALLBACK(onSignal), (gpointer) callback);
    }

    return args.This();
  }

  // Show()
  // For showing the widget.
  static inline v8::Handle<v8::Value> Show (const v8::Arguments &args) {
    v8::HandleScope scope;

    GtkWidget *widget = Widget::Gtk(args.This());

    gtk_widget_show_all(widget);

    return args.This();
  }

  // For destroying widgets
  static inline v8::Handle<v8::Value> Destroy (const v8::Arguments &args) {
    v8::HandleScope scope;

    GtkWidget *widget = node::ObjectWrap::Unwrap<Widget>(args.This())->widget_;

    gtk_widget_destroy(widget);

    return args.This();
  }

  // Event handler.
  static inline void onSignal (GtkWidget *widget, gpointer callback_ptr) {
    v8::HandleScope scope;

    v8::Persistent<v8::Function> *callback = reinterpret_cast<v8::Persistent<v8::Function>*>(callback_ptr);

    v8::TryCatch try_catch;

    (*callback)->Call(v8::Context::GetCurrent()->Global(), 0, NULL);

    if (try_catch.HasCaught()) {
      node::FatalException(try_catch);
    }
  }
};

} // namespace ngtk

#endif

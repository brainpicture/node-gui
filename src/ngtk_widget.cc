#include "ngtk_widget.h"

#include <v8.h>
#include <node.h>

#include <gtk/gtk.h>          // GtkWidget
#include <node_object_wrap.h> // ObjectWrap

#include "ngtk.h"

namespace ngtk {

using namespace v8;
using namespace node;

// Add signal handler.
Handle<Value> Widget::On (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString() && args[1]->IsFunction()) {
    GtkWidget *widget = ObjectWrap::Unwrap<Widget>(args.This())->widget_;

    Persistent<Function> *callback = new Persistent<Function>();
    *callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));

    g_signal_connect(G_OBJECT(widget), *String::Utf8Value(args[0]->ToString()),
        G_CALLBACK(Widget::onSignal), (gpointer) callback);
  }

  return args.This();
}

// Show()
// For showing the widget.
Handle<Value> Widget::Show (const Arguments &args) {
  HandleScope scope;

  GtkWidget *widget = ObjectWrap::Unwrap<Widget>(args.This())->widget_;

  gtk_widget_show_all(widget);

  return args.This();
}

// For destroying widgets
Handle<Value> Widget::Destroy (const Arguments &args) {
  HandleScope scope;

  GtkWidget *widget = ObjectWrap::Unwrap<Widget>(args.This())->widget_;

  gtk_widget_destroy(widget);

  return args.This();
}

// Event handler.
void Widget::onSignal (GtkWidget *widget, gpointer callback_ptr) {
  HandleScope scope;

  Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(callback_ptr);

  TryCatch try_catch;

  (*callback)->Call(Context::GetCurrent()->Global(), 0, NULL);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }
}

void Widget::Initialize (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "on",      Widget::On);
  // Maybe over-riden by Window for ev_ref etc.
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "show",    Widget::Show);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "destroy", Widget::Destroy);
}


} // namespace ngtk

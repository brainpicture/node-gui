#ifndef NGTK_WIDGET_H_
#define NGTK_WIDGET_H_

#include <node_object_wrap.h> // node::ObjectWrap
#include "ngtk.h"

namespace ngtk {

class Widget : public node::ObjectWrap {
public:
  static void SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);

  // For getting the underlying GtkWidget
  static inline GtkWidget* Data (v8::Handle<v8::Object> obj) {
    v8::HandleScope scope;
    return ObjectWrap::Unwrap<Widget>(obj)->widget_;
  }

  GtkWidget *widget_;

private:
  // Add signal handler.
  static v8::Handle<v8::Value> On      (const v8::Arguments &args);
  // For showing the widget.
  static v8::Handle<v8::Value> Show    (const v8::Arguments &args);
  // For destroying widgets
  static v8::Handle<v8::Value> Destroy (const v8::Arguments &args);
  // Event handler.
  // TODO: Improve how we handle signals. Signals have variable arguments...
  static void onSignal (GtkWidget *widget, gpointer callback_ptr);
};

} // namespace ngtk

#endif

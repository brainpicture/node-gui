#ifndef NGTK_WIDGET_H_
#define NGTK_WIDGET_H_

#include <node_object_wrap.h> // node::ObjectWrap
#include "v8.h"
#include "ngtk.h"
#include <vector>

namespace ngtk {

struct SignalCallback {
  const char *name;
  GCallback callback;
  
  SignalCallback (const char *name, GCallback callback) {
    this->name = name;
    this->callback = callback;
  }
};

class Widget : public node::ObjectWrap {
public:
  static void SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);
  static void RegisterCallbacks   (std::vector<SignalCallback> *callbacks);

  // For getting the underlying GtkWidget
  static inline GtkWidget* Data (v8::Handle<v8::Object> obj) {
    v8::HandleScope scope;
    return ObjectWrap::Unwrap<Widget>(obj)->widget_;
  }

  GtkWidget *widget_;

  static std::vector<SignalCallback> *callbacks;

  // Event handlers.
  static void     SignalBare   (GtkWidget *widget, gpointer callback_ptr);
  static gboolean SignalBoolean(GtkWidget *widget, gpointer callback_ptr);
  static gboolean SignalFocus  (GtkWidget *widget, GtkDirectionType direction, gpointer callback_ptr);

private:
  // Add signal handler.
  static v8::Handle<v8::Value> On         (const v8::Arguments &args);
  // remove signal handler.
  static v8::Handle<v8::Value> Disconnect (const v8::Arguments &args);

  // For showing the widget.
  static v8::Handle<v8::Value> Show       (const v8::Arguments &args);
  // For destroying widgets
  static v8::Handle<v8::Value> Destroy    (const v8::Arguments &args);

  // For checking focus
  static v8::Handle<v8::Value> IsFocus    (const v8::Arguments &args);
  // For grabbing focus
  static v8::Handle<v8::Value> GrabFocus  (const v8::Arguments &args);
};


} // namespace ngtk

#endif

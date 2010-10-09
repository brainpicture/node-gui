#ifndef NGTK_WIDGET_H_
#define NGTK_WIDGET_H_

#include <v8.h>

#include "ngtk.h"
#include <gtk/gtk.h>          // GtkWidget
#include <node_object_wrap.h> // node::ObjectWrap

namespace ngtk {

class Widget : public node::ObjectWrap {
public:
  // For getting the underlying GtkWidget
  static inline GtkWidget* Gtk (v8::Handle<v8::Object> obj) {
    v8::HandleScope scope;

    return node::ObjectWrap::Unwrap<Widget>(obj)->widget_;
  }

  GtkWidget *widget_;

protected:
  // onDestroy() - Called when the window is being destoyed.
  static inline void onDestroy (GtkWidget *widget, gpointer dataCast) {
    v8::Persistent<v8::Object> *self      = reinterpret_cast<v8::Persistent<v8::Object>*>(dataCast);
    v8::Handle<v8::Value>       onDestroy = (*self)->Get(v8::String::New("onDestroy"));

    if (onDestroy->IsFunction()) {
      v8::Handle<v8::Function>::Cast(onDestroy)->Call(*self, 0, NULL);
    }
  }
};

} // namespace ngtk

#endif

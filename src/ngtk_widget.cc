#include "ngtk_widget.h"

#include <v8.h>
#include <gtk/gtk.h>
#include "ngtk.h"

namespace ngtk {

using namespace v8;

Widget::~Widget (void) {
  gtk_widget_destroy(widget_);
}

// For getting the underlying GtkWidget
GtkWidget* Widget::Gtk (Handle<Object> obj) {
  HandleScope scope;

  return ObjectWrap::Unwrap<Widget>(obj)->widget_;
}

// onDestroy() - Called when the window is being destoyed.
void Widget::onDestroy (GtkWidget *widget, gpointer dataCast) {
  Persistent<Object>   *self      = reinterpret_cast<Persistent<Object>*>(dataCast);
  v8::Handle<v8::Value> onDestroy = (*self)->Get(String::New("onDestroy"));

  if (onDestroy->IsFunction()) {
    Handle<Function>::Cast(onDestroy)->Call(*self, 0, NULL);
  }

  main_loop_level--;
}

} // namespace ngtk

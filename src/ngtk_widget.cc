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

} // namespace ngtk

#ifndef NGTK_WIDGET_H_
#define NGTK_WIDGET_H_

#include <v8.h>

#include "ngtk.h"
#include <gtk/gtk.h>          // GtkWidget
#include <node_object_wrap.h> // node::ObjectWrap

namespace ngtk {

class Widget : public node::ObjectWrap {
public:
  ~Widget();
  static GtkWidget* Gtk (v8::Handle<v8::Object> obj);

protected:
  GtkWidget *widget_;
};

} // namespace ngtk

#endif

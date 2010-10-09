#ifndef NGTK_BUTTON_H_
#define NGTK_BUTTON_H_

#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_widget.h"

namespace ngtk {

class Button : public Widget {
public:
  static void       Initialize (v8::Handle<v8::Object> target);
  static Button*    New        (void); // public constructor

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New      (const v8::Arguments &args);
  // gtk_button_*_label()
  static v8::Handle<v8::Value> SetLabel (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetLabel (const v8::Arguments &args);
  // onClick()
  static void                  onClick  (GtkWidget *widget, gpointer dataCast);

  Button(void);

};

} // namespace ngtk

#endif

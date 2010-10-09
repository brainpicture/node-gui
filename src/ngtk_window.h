#ifndef NGTK_WINDOW_H_
#define NGTK_WINDOW_H_

#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_widget.h"

namespace ngtk {

class Window : public Widget {
public:
  static void       Initialize (v8::Handle<v8::Object> target);
  static Window*    New        (void); // public constructor

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New            (const v8::Arguments &args);
  static v8::Handle<v8::Value> Add            (const v8::Arguments &args);
  static v8::Handle<v8::Value> Show           (const v8::Arguments &args);
  // gtk_window_*_title()
  static v8::Handle<v8::Value> SetTitle       (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetTitle       (const v8::Arguments &args);
  // gtk_window_*_resizable()
  static v8::Handle<v8::Value> SetResizable   (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetResizable   (const v8::Arguments &args);
  // gtk_window_set_default_size() gtk_window_get_size()
  static v8::Handle<v8::Value> SetDefaultSize (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetSize        (const v8::Arguments &args);
  // gtk_window_*_position()
  static v8::Handle<v8::Value> SetPosition    (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetPosition    (const v8::Arguments &args);
  // gtk_window_*_opacity()
  static v8::Handle<v8::Value> SetOpacity     (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetOpacity     (const v8::Arguments &args);
  // onDestroy()
  static void                  onDestroy      (GtkWidget *widget, gpointer dataCast);

  Window(void);

};

} // namespace ngtk

#endif

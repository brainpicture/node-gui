#ifndef NGTK_HBOX_H_
#define NGTK_HBOX_H_

#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_container.h"

namespace ngtk {

class Hbox : public Container {
public:
  static void  Initialize  (v8::Handle<v8::Object> target);
  static Hbox* New         (bool homogeneous, int spacing); // public constructor
  static bool  HasInstance (v8::Handle<v8::Value> val);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New (const v8::Arguments &args);
  static v8::Handle<v8::Value> Add (const v8::Arguments &args);

  Hbox(gboolean homogeneous, gint spacing);
};

} // namespace ngtk

#endif

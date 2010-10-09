#ifndef NGTK_WINDOW_H_
#define NGTK_WINDOW_H_

#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_widget.h"

namespace ngtk {

class MessageDialog : public Widget {
public:
  static void           Initialize (v8::Handle<v8::Object> target);
  static MessageDialog* New        (void); // public constructor

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New (const v8::Arguments &args);

  MessageDialog(void);
};

} // namespace ngtk

#endif

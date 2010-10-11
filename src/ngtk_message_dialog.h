#ifndef NGTK_MESSAGE_DIALOG_H_
#define NGTK_MESSAGE_DIALOG_H_

#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_widget.h"
#include "ngtk_window.h"

namespace ngtk {

class MessageDialog : public Widget {
public:
  static void           Initialize  (v8::Handle<v8::Object> target);
  static MessageDialog* New         (Window *parent, GtkDialogFlags flags,
      GtkMessageType type, GtkButtonsType buttons, char *message); // public constructor
  static bool           HasInstance (v8::Handle<v8::Value> val);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New (const v8::Arguments &args);

  MessageDialog(GtkWindow *parent, GtkDialogFlags flags,
      GtkMessageType type, GtkButtonsType buttons, char *message);
};

} // namespace ngtk

#endif

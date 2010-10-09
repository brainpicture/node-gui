#include "ngtk.h"
#include <v8.h>

#include <gtk/gtk.h>
#include "ngtk_window.h"
#include "ngtk_message_dialog.h"
#include "ngtk_button.h"

namespace ngtk {

using namespace v8;

//global var
int main_loop_level = 0;

Handle<Value> MainIteration(const Arguments& args) {
  while (gtk_events_pending()) {
    gtk_main_iteration();
  }

  return Boolean::New(main_loop_level == 0);
}

extern "C" void init (Handle<Object> target) {
  HandleScope scope;

  gtk_init(NULL, NULL);

  // Position constants.
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_NONE",             GTK_WIN_POS_NONE);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_CENTER",           GTK_WIN_POS_CENTER);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_MOUSE",            GTK_WIN_POS_MOUSE);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_CENTER_ALWAYS",    GTK_WIN_POS_CENTER_ALWAYS);
  NGTK_DEFINE_CONSTANT(target, "WIN_POS_CENTER_ON_PARENT", GTK_WIN_POS_CENTER_ON_PARENT);

  // MessageDialog constants
  NGTK_DEFINE_CONSTANT(target, "DIALOG_MODAL",               GTK_DIALOG_MODAL);
  NGTK_DEFINE_CONSTANT(target, "DIALOG_DESTROY_WITH_PARENT", GTK_DIALOG_DESTROY_WITH_PARENT);
  NGTK_DEFINE_CONSTANT(target, "DIALOG_NO_SEPARATOR",        GTK_DIALOG_NO_SEPARATOR);

  // Message types.
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_INFO",     GTK_MESSAGE_INFO);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_WARNING",  GTK_MESSAGE_WARNING);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_QUESTION", GTK_MESSAGE_QUESTION);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_ERROR",    GTK_MESSAGE_ERROR);
  NGTK_DEFINE_CONSTANT(target, "MESSAGE_OTHER",    GTK_MESSAGE_OTHER);

  // Buttons.
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_NONE",      GTK_BUTTONS_NONE);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_OK",        GTK_BUTTONS_OK);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_CLOSE",     GTK_BUTTONS_CLOSE);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_CANCEL",    GTK_BUTTONS_CANCEL);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_YES_NO",    GTK_BUTTONS_YES_NO);
  NGTK_DEFINE_CONSTANT(target, "BUTTONS_OK_CANCEL", GTK_BUTTONS_OK_CANCEL);

  // Response constants
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_NONE",         GTK_RESPONSE_NONE);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_REJECT",       GTK_RESPONSE_REJECT);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_ACCEPT",       GTK_RESPONSE_ACCEPT);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_DELETE_EVENT", GTK_RESPONSE_DELETE_EVENT);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_OK",           GTK_RESPONSE_OK);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_CANCEL",       GTK_RESPONSE_CANCEL);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_CLOSE",        GTK_RESPONSE_CLOSE);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_YES",          GTK_RESPONSE_YES);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_NO",           GTK_RESPONSE_NO);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_APPLY",        GTK_RESPONSE_APPLY);
  NGTK_DEFINE_CONSTANT(target, "RESPONSE_HELP",         GTK_RESPONSE_HELP);

  Window::Initialize(target);
  MessageDialog::Initialize(target);
  Button::Initialize(target);

  NGTK_SET_METHOD(target, "main", MainIteration);
}

} // namespace ngtk

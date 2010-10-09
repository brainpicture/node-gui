#include "ngtk.h"
#include <v8.h>

#include <gtk/gtk.h>
#include "ngtk_window.h"
#include "ngtk_message_dialog.h"

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
  NGTK_DEFINE_CONSTANT(target, "DIALOG_NO_SEPERATOR",        GTK_DIALOG_NO_SEPERATOR);

  Window::Initialize(target);

  NGTK_SET_METHOD(target, "main", MainIteration);
}

} // namespace ngtk

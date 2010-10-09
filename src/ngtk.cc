#include "ngtk.h"
#include <v8.h>

#include <gtk/gtk.h>
#include "ngtk_window.h"

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

  Window::Initialize(target);

  NGTK_SET_METHOD(target, "main", MainIteration);
}

} // namespace ngtk

#include <v8.h>
using namespace v8;

Handle<Value> window_show(const Arguments& args);
Handle<Value> window_setTitle(const Arguments& args);
Handle<Value> window_setSize(const Arguments& args);
Handle<Value> window_add(const Arguments& args);
void window_onClose_callback(GtkWidget *widget, gpointer dataCast);
Handle<Value> window_onClose(const Arguments& args);
Handle<Value> window(const Arguments& args);

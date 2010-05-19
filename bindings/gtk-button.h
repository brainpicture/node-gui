#include <v8.h>
using namespace v8;

//Button
Handle<Value> button_setTitle(const Arguments& args);
void button_onClick_callback(GtkWidget *widget, gpointer dataCast);
Handle<Value> button_onClick(const Arguments& args);
Handle<Value> button(const Arguments& args);

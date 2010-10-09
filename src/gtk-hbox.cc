#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

Handle<Value> HboxAdd (const Arguments& args) {
  HandleScope scope;

  GtkWidget *hbox;
  GTKNODE_GET_HANDLE(hbox, args.This());

  GtkWidget *obj;
  GTKNODE_GET_HANDLE(obj, args[0]->ToObject());

  gtk_container_add(GTK_CONTAINER(hbox), obj);
  gtk_widget_show(obj);

  return args.This();
}

Handle<Value> Hbox (const Arguments& args) {
  HandleScope scope;

  Local<Object> hboxButton = Object::New();

  GtkWidget *hbox = gtk_hbox_new(false, 0);

  hboxButton->Set(String::New("handle"),  v8::External::New(hbox));
  hboxButton->Set(String::New("add"), FunctionTemplate::New(HboxAdd)->GetFunction());

  return scope.Close(hboxButton);
}

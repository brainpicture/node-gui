#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

Handle<Value> VboxAdd (const Arguments& args) {
  HandleScope scope;

  GtkWidget *vbox;
  GTKNODE_GET_HANDLE(vbox, args.This());

  GtkWidget *obj;
  GTKNODE_GET_HANDLE(obj, args[0]->ToObject());

  gtk_container_add(GTK_CONTAINER(vbox), obj);
  gtk_widget_show(obj);

  return args.This();
}

Handle<Value> Vbox (const Arguments& args) {
  HandleScope scope;

  Local<Object> vboxButton = Object::New();

  GtkWidget *vbox = gtk_vbox_new(false, 0);

  vboxButton->Set(String::New("handle"),  v8::External::New(vbox));
  vboxButton->Set(String::New("add"), FunctionTemplate::New(VboxAdd)->GetFunction());

  return scope.Close(vboxButton);
}

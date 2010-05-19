#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

// Vbox ----------------------------------
Handle<Value>
VBox_add(const Arguments& args)
{
  Local<Object> vboxButton = args.This();
  GtkWidget *vbox = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(vboxButton->Get(String::New("handle")))->Value());
  
  
  Local<Object> otherObject = args[0]->ToObject();
  GtkWidget *obj = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(otherObject->Get(String::New("handle")))->Value());
  
  gtk_container_add(GTK_CONTAINER(vbox), obj);
  gtk_widget_show(obj);
  return vboxButton;
}


Handle<Value>
VBox(const Arguments& args)
{
  HandleScope scope;
  Local<Object> vboxButton = Object::New();
  
  GtkWidget *vbox = gtk_vbox_new(false, 0);
  
  vboxButton->Set(String::New("handle"),  v8::External::New(vbox));
  vboxButton->Set(String::New("add"), FunctionTemplate::New(VBox_add)->GetFunction());
  
  return vboxButton;
}

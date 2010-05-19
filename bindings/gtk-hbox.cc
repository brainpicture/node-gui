#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

// Hbox ----------------------------------
Handle<Value>
HBox_add(const Arguments& args)
{
  Local<Object> hboxButton = args.This();
  GtkWidget *hbox = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(hboxButton->Get(String::New("handle")))->Value());
  
  
  Local<Object> otherObject = args[0]->ToObject();
  GtkWidget *obj = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(otherObject->Get(String::New("handle")))->Value());
  
  gtk_container_add(GTK_CONTAINER(hbox), obj);
  gtk_widget_show(obj);
  return hboxButton;
}


Handle<Value>
HBox(const Arguments& args)
{
  HandleScope scope;
  Local<Object> hboxButton = Object::New();
  
  GtkWidget *hbox = gtk_hbox_new(false, 0);
  
  hboxButton->Set(String::New("handle"),  v8::External::New(hbox));
  hboxButton->Set(String::New("add"), FunctionTemplate::New(HBox_add)->GetFunction());
  
  return hboxButton;
}

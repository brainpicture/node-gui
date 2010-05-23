#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

Handle<Value> entry_set_text(const Arguments& args) {
    Local<Object> entryObject = args.This();
    GtkWidget *entry = static_cast<GtkWidget*>(Handle<External>::Cast(entryObject->Get(String::New("handle")))->Value());
  
    String::Utf8Value text(args[0]->ToString());
    gtk_entry_set_text(GTK_ENTRY(entry), *text);
}

Handle<Value> entry(const Arguments& args) {
    HandleScope scope;
    Local<Object> entryObject = Object::New();
  
    GtkWidget *entry = gtk_entry_new();
  
    entryObject->Set(String::New("handle"),  External::New(entry));
    entryObject->Set(String::New("set_text"),  FunctionTemplate::New(entry_set_text)->GetFunction());
  
    return entryObject;
}

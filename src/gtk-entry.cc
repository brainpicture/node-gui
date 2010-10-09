#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

Handle<Value> EntrySetText (const Arguments& args) {
  HandleScope scope;

  GtkWidget *entry;
  GTKNODE_GET_HANDLE(entry, args.This());

  gtk_entry_set_text(GTK_ENTRY(entry), *(String::Utf8Value(args[0]->ToString())));

  return args.This();
}

Handle<Value> Entry (const Arguments& args) {
  HandleScope scope;

  Local<Object> entryObject = Object::New();

  GtkWidget *entry = gtk_entry_new();

  entryObject->Set(String::New("handle"),  External::New(entry));
  entryObject->Set(String::New("setText"),  FunctionTemplate::New(EntrySetText)->GetFunction());

  return scope.Close(entryObject);
}

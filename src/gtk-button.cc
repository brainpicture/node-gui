#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

Handle<Value> ButtonSetTitle (const Arguments& args) {
  HandleScope scope;

  GtkWidget *button;
  GTKNODE_GET_HANDLE(button, args.This());

  gtk_button_set_label(GTK_BUTTON(button),
      *String::Utf8Value(args[0]->ToString()));

  return args.This();
}

void buttonOnClickCallback (GtkWidget *widget, gpointer dataCast) {
  Persistent<Object> *data = reinterpret_cast<Persistent<Object>*>(dataCast);

  Handle<Value> onClick = (*data)->Get(String::New("onClick"));

  if (onClick->IsFunction()) {
    Handle<Function>::Cast(onClick)->Call((*data), 0, NULL);
  }
}

Handle<Value> ButtonOnClick (const Arguments& args) {
  HandleScope scope;

  GtkWidget *button;
  GTKNODE_GET_HANDLE(button, args.This());

  args.This()->Set(String::New("onClick"), args[0]);

  Persistent<Object> *data = new Persistent<Object>();
  *data = Persistent<Object>::New(args.This());

  g_signal_connect(G_OBJECT(button), "clicked",
      G_CALLBACK(buttonOnClickCallback), (gpointer) data);

  return args.This();
}

Handle<Value> Button (const Arguments& args) {
  HandleScope scope;

  Local<Object> buttonObject = Object::New();

  GtkWidget *btn = gtk_button_new();

  buttonObject->Set(String::New("handle"),  External::New(btn));
  buttonObject->Set(String::New("setTitle"), FunctionTemplate::New(ButtonSetTitle)->GetFunction());
  buttonObject->Set(String::New("onClick"), FunctionTemplate::New(ButtonOnClick)->GetFunction());

  return scope.Close(buttonObject);
}

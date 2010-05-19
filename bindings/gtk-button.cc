#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

// Button ----------------------------------
Handle<Value>
button_setTitle(const Arguments& args)
{
  Local<Object> buttonObject = args.This();
  GtkWidget *btn = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(buttonObject->Get(String::New("handle")))->Value());
  
  String::Utf8Value title(args[0]->ToString());
  gtk_button_set_label(GTK_BUTTON(btn), *title);
}

void
button_onClick_callback(GtkWidget *widget, gpointer dataCast) {
  HandleScope scope;
  Persistent<Object> *data = reinterpret_cast<Persistent<Object>*>(dataCast);
  v8::Handle<v8::Value> onClose = (*data)->Get(String::New("onClick"));
  if (onClose->IsFunction()) {
	v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(onClose);
	v8::Handle<v8::Value> emptyArgs[] = {};
	callback->Call((*data), 0, emptyArgs);
  }
}

Handle<Value>
button_onClick(const Arguments& args)
{
  Local<Object> buttonObject = args.This();
  GtkWidget *btn = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(buttonObject->Get(String::New("handle")))->Value());
  
  buttonObject->Set(String::New("onClick"), args[0]);
  
  Persistent<Object> *data = new Persistent<Object>();
  *data = Persistent<Object>::New(buttonObject);
  
  g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(button_onClick_callback), (gpointer) data);
  return buttonObject;
}

Handle<Value>
button(const Arguments& args)
{
  HandleScope scope;
  Local<Object> buttonObject = Object::New();
  
  GtkWidget *btn = gtk_button_new();
  
  buttonObject->Set(String::New("handle"),  v8::External::New(btn));
  buttonObject->Set(String::New("setTitle"), FunctionTemplate::New(button_setTitle)->GetFunction());
  buttonObject->Set(String::New("onClick"), FunctionTemplate::New(button_onClick)->GetFunction());
  
  return buttonObject;
}

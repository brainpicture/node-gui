#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

Handle<Value> button_setTitle(const Arguments& args) {
    Local<Object> buttonObject = args.This();
    GtkWidget *btn = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(buttonObject->Get(String::New("handle")))->Value());
  
    String::Utf8Value title(args[0]->ToString());
    gtk_button_set_label(GTK_BUTTON(btn), *title);
}

void button_on_click_callback(GtkWidget *widget, gpointer dataCast) {
    HandleScope scope;
    Persistent<Object> *data = reinterpret_cast<Persistent<Object>*>(dataCast);
    v8::Handle<v8::Value> on_click = (*data)->Get(String::New("on_click"));
    if (on_click->IsFunction()) {
	v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(on_click);
	v8::Handle<v8::Value> emptyArgs[] = {};
	callback->Call((*data), 0, emptyArgs);
    }
}

Handle<Value> button_on_click(const Arguments& args) {
    Local<Object> buttonObject = args.This();
    GtkWidget *btn = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(buttonObject->Get(String::New("handle")))->Value());
  
    buttonObject->Set(String::New("on_click"), args[0]);
  
    Persistent<Object> *data = new Persistent<Object>();
    *data = Persistent<Object>::New(buttonObject);
  
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(button_on_click_callback), (gpointer) data);
    return buttonObject;
}

Handle<Value> button(const Arguments& args) {
    HandleScope scope;
    Local<Object> buttonObject = Object::New();
  
    GtkWidget *btn = gtk_button_new();
  
    buttonObject->Set(String::New("handle"),  v8::External::New(btn));
    buttonObject->Set(String::New("set_title"), FunctionTemplate::New(button_set_title)->GetFunction());
    buttonObject->Set(String::New("on_click"), FunctionTemplate::New(button_on_click)->GetFunction());
  
    return buttonObject;
}

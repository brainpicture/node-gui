#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
#include <stdio.h>
using namespace v8;

// Window ----------------------------------

Handle<Value>
window_show(const Arguments& args)
{
  HandleScope scope;
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  gtk_widget_show_all(wnd);
  
  std::cout << "Loop level: " << main_loop_level;
  
  main_loop_level++;
  std::cout << "Loop level: " << main_loop_level;
  if (main_loop_level == 1) {
  	v8::Handle<v8::Value> onShow = windowObject->Get(String::New("onShow"));
    if (onShow->IsFunction()) {
	  v8::Handle<v8::Value> emptyArgs[] = {};
	  v8::Handle<v8::Function>::Cast(onShow)->Call(windowObject, 0, emptyArgs);
    }
  }
  
  return windowObject;
}

Handle<Value>
window_setTitle(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  String::Utf8Value title(args[0]->ToString());
  gtk_window_set_title(GTK_WINDOW(wnd), *title);
  return windowObject;
}

Handle<Value>
window_setSize(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  int width,height;
  if (args[0]->IsNumber())
  	width=args[0]->ToInteger()->Value();
  else
  	width=640;
  
  if (args[1]->IsNumber())
  	height=args[1]->ToInteger()->Value();
  else
  	height=480;
  gtk_window_set_default_size(GTK_WINDOW(wnd),width,height);
  return windowObject;
}

Handle<Value>
window_add(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  
  Local<Object> otherObject = args[0]->ToObject();
  GtkWidget *obj = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(otherObject->Get(String::New("handle")))->Value());
  
  gtk_container_add(GTK_CONTAINER (wnd), obj);
  gtk_widget_show(obj);
  return windowObject;
}

void
window_onClose_callback(GtkWidget *widget, gpointer dataCast) {
  HandleScope scope;
  Persistent<Object> *data = reinterpret_cast<Persistent<Object>*>(dataCast);
  v8::Handle<v8::Value> onClose = (*data)->Get(String::New("onClose"));
  if (onClose->IsFunction()) {
	v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(onClose);
	v8::Handle<v8::Value> emptyArgs[] = {};
	callback->Call((*data), 0, emptyArgs);
  }
  main_loop_level--;
}

Handle<Value>
window_onClose(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  windowObject->Set(String::New("onClose"), args[0]);

  Persistent<Object> *data = new Persistent<Object>();
  *data = Persistent<Object>::New(windowObject);
  
  g_signal_connect(G_OBJECT(wnd), "destroy", G_CALLBACK(window_onClose_callback), (gpointer) data);
  return windowObject;
}

Handle<Value>
window(const Arguments& args)
{
  HandleScope scope;
  
  Local<Object> windowObject = Object::New();
  
  GtkWidget *wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  Local<Object> params = args[0]->ToObject();
  
  
  windowObject->Set(String::New("handle"),  v8::External::New(wnd));

  windowObject->Set(String::New("show"), FunctionTemplate::New(window_show)->GetFunction());
  windowObject->Set(String::New("setTitle"), FunctionTemplate::New(window_setTitle)->GetFunction());
  windowObject->Set(String::New("setSize"), FunctionTemplate::New(window_setSize)->GetFunction());
  windowObject->Set(String::New("add"), FunctionTemplate::New(window_add)->GetFunction());
  windowObject->Set(String::New("onClose"), FunctionTemplate::New(window_onClose)->GetFunction());
  
  return windowObject;
}

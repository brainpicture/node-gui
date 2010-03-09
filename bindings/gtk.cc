#include <v8.h>

#include <gtk/gtk.h>

#include <iostream>
using namespace v8;

// Window ----------------------------------
Handle<Value>
window_show(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());
  
  gtk_widget_show(wnd);
  gtk_main();
}

Handle<Value>
window_setTitle(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());
  
  String::Utf8Value title(args[0]->ToString());
  gtk_window_set_title(GTK_WINDOW (wnd), *title);
}


Handle<Value>
window(const Arguments& args)
{
  HandleScope scope;	
  
  Local<Object> windowObject = Object::New();
  
  GtkWidget *wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  Local<Object> params = args[0]->ToObject();
  
  
  Local<Integer> width;
  Local<Integer> height;
  if (params->Has(String::New("width")))
  	width=params->Get(String::New("width"))->ToInteger();
  else
  	width=Integer::New(640);
  
  if (params->Has(String::New("height")))
  	height=params->Get(String::New("height"))->ToInteger();
  else
  	height=Integer::New(480);
  
  gtk_window_set_default_size (GTK_WINDOW (wnd),width->Value(),height->Value());
  
  windowObject->Set(String::New("windowHandle"),  v8::External::New(wnd));

  windowObject->Set(String::New("show"), FunctionTemplate::New(window_show)->GetFunction());
  windowObject->Set(String::New("setTitle"), FunctionTemplate::New(window_setTitle)->GetFunction());
  return windowObject;
}


extern "C" void init (Handle<Object> target)
{
  HandleScope scope;
  gtk_init(0,(char***)"");
  target->Set(String::New("window"), FunctionTemplate::New(window)->GetFunction());
}

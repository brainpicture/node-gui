#include <v8.h>

#include <gtk/gtk.h>

#include <iostream>
using namespace v8;

class gtk_data {
  public:
	Local<Object> windowObject;
	Handle<Object> recv;
};

int main_loop_level = 0;

Handle<Value>
main_iteration(const Arguments& args)
{
  if (gtk_events_pending()) {
  	gtk_main_iteration();
  }
  
  return Boolean::New(false);
}

// Window ----------------------------------

Handle<Value>
window_show(const Arguments& args)
{
  HandleScope scope;
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  gtk_widget_show(wnd);
  
  main_loop_level++;
  
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
  
  
  Local<Object> buttonObject = args[0]->ToObject();
  GtkWidget *btn = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(buttonObject->Get(String::New("handle")))->Value());
  
  gtk_container_add (GTK_CONTAINER (wnd), btn);
  gtk_widget_show (btn);
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
  
  //Local<Object> params = args[0]->ToObject();
  return buttonObject;
}



extern "C" void init (Handle<Object> target)
{
  HandleScope scope;
  gtk_init(NULL, NULL);
  target->Set(String::New("window"), FunctionTemplate::New(window)->GetFunction());
  target->Set(String::New("button"), FunctionTemplate::New(button)->GetFunction());

  target->Set(String::New("main"), FunctionTemplate::New(main_iteration)->GetFunction());
}

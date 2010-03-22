#include <v8.h>

#include <gtk/gtk.h>

#include <iostream>
using namespace v8;

class gtk_data {
  public:
	Local<Object> windowObject;
	Handle<Object> recv;
};


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
window_onClose(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());

  v8::Local<v8::Object> arguments = v8::Object::New();
  arguments->Set(String::New("callback"),args[0]);
  Persistent<Object> *data = new Persistent<Object>();
  *data = Persistent<Object>::New(arguments);
  //Persistent data = reinterpret_cast<Persistent>(windowObject);
 
}

void
window_destroy(GtkWidget widget, void* data)
{
	printf("CALLBACK: \n");
	//gtk_data *fd=(gtk_data *)data;
	//fd->windowObject->Get(String::New("onClose"));
	Persistent<Object> *windowObject = reinterpret_cast<Persistent<Object>*>(data);
	
	v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast((*windowObject)->Get(String::New("onClose")));
    //Handle<Object> recv = Handle<Object>::Cast(fd->recv);
    v8::Handle<v8::Value> outArgs[] = {String::New((char *)"wow",32)};
    //callback->Call(recv, 1, outArgs);
    callback->Call((*windowObject), 1, outArgs);
	
/*  Handle callback;
  Handle argv[] = { (data) };
  callback = Handle::Cast((data)->Get(String::New("onClose")));
  callback->Call((*data), 1, argv);*/
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
  //windowObject->Set(String::New("onClose"), FunctionTemplate::New(window_onClose)->GetFunction());
  
  
  Persistent<Object> *data = new Persistent<Object>();
	  *data = Persistent<Object>::New(windowObject);
  
  //gtk_data *gd=new gtk_data;  
  //gd->windowObject=data;
  
  g_signal_connect (G_OBJECT (wnd), "destroy", G_CALLBACK (window_destroy), static_cast<void*>(data));
  
  
  return windowObject;
}


extern "C" void init (Handle<Object> target)
{
  HandleScope scope;
  gtk_init(0,(char***)"");
  target->Set(String::New("window"), FunctionTemplate::New(window)->GetFunction());
}

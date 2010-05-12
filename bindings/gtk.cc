#include <v8.h>

#include <gtk/gtk.h>

#include <iostream>
using namespace v8;

class gtk_data {
  public:
	Local<Object> windowObject;
	Handle<Object> recv;
};

Handle<Value>
main_iteration(const Arguments& args)
{
  if (gtk_events_pending()) {
  	gtk_main_iteration();
  }
  return Boolean::New(true);
}

void*
fake_main_iteration(void* fakedata) {
	Persistent<Object> *data2 = reinterpret_cast<Persistent<Object>*>(fakedata);
	std::cout << "fake signal\n";
	//Persistent<Object> *data = new Persistent<Object>();
	//*data = Persistent<Object>::New(windowObject);
	
		v8::Handle<v8::Value> onClose = (*data2)->Get(String::New("onClose"));
		if (onClose->IsFunction()) {
			std::cout << "step 1\n";
			v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(onClose);
			std::cout << "step 2\n";
			v8::Handle<v8::Value> emptyArgs[] = {};
			std::cout << "step 3\n";
			callback->Call((*data2), 0, emptyArgs);
			std::cout << "step 4 ok!!!!!\n";
		}

}

// Window ----------------------------------
Handle<Value>
window_show(const Arguments& args)
{
  HandleScope scope;
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());
  
  gtk_widget_show(wnd);
  return windowObject;
}

Handle<Value>
window_setTitle(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());
  
  String::Utf8Value title(args[0]->ToString());
  gtk_window_set_title(GTK_WINDOW(wnd), *title);
  return windowObject;
}

Handle<Value>
window_setSize(const Arguments& args)
{
  Local<Object> windowObject = args.This();
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());
  
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
  GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("windowHandle")))->Value());
  
  
  Local<Object> buttonObject = args[0]->ToObject();
  GtkWidget *btn = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(buttonObject->Get(String::New("handle")))->Value());
  
  gtk_container_add (GTK_CONTAINER (wnd), btn);
  gtk_widget_show (btn);
  return windowObject;
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
window_destroy(GtkWidget widget, void* dataCast)
{
	HandleScope scope;
	printf("CALLBACK: \n");
	
	Persistent<Object> *data = reinterpret_cast<Persistent<Object>*>(dataCast);
	
	//Local<Object> windowObject = (*windowObjectPer);
	/*
	
	v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast((*windowObject)->Get(String::New("onClose")));

	Handle<Object> recv = Handle<Object>::Cast((*windowObject)->Get(String::New("recv")));
	
    v8::Handle<v8::Value> emptyArgs[] = {};
    
    std::cout << "still alive\n";
    callback->Call(recv, 0, emptyArgs);
    std::cout << "not at  all\n";
*/


		v8::Handle<v8::Value> onClose = (*data)->Get(String::New("onClose"));
		if (onClose->IsFunction()) {
			std::cout << "step 1\n";
			v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(onClose);
			std::cout << "step 2\n";
			v8::Handle<v8::Value> emptyArgs[] = {};
			std::cout << "step 3\n";
			callback->Call((*data), 0, emptyArgs);
			std::cout << "step 4\n";
		}
		std::cout << "no\n";
}

Handle<Value>
window(const Arguments& args)
{
  HandleScope scope;
  
  Local<Object> windowObject = Object::New();
  
  GtkWidget *wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  Local<Object> params = args[0]->ToObject();
  
  
  windowObject->Set(String::New("windowHandle"),  v8::External::New(wnd));

  windowObject->Set(String::New("show"), FunctionTemplate::New(window_show)->GetFunction());
  windowObject->Set(String::New("setTitle"), FunctionTemplate::New(window_setTitle)->GetFunction());
  windowObject->Set(String::New("setSize"), FunctionTemplate::New(window_setSize)->GetFunction());
  windowObject->Set(String::New("add"), FunctionTemplate::New(window_add)->GetFunction());
  //windowObject->Set(String::New("onClose"), FunctionTemplate::New(window_onClose)->GetFunction());
  

  windowObject->Set(String::New("onClose"), params->Get(String::New("onClose")));
  
  Persistent<Object> *data = new Persistent<Object>(windowObject);
	void* retrans = static_cast<void*>(data);
	Persistent<Object> *data2 = reinterpret_cast<Persistent<Object>*>(retrans);
	//Persistent<Object> *data = new Persistent<Object>();
	//  *data = Persistent<Object>::New(windowObject);
	
	if (params->Has(String::New("onClose"))) {
		v8::Handle<v8::Value> onClose = (*data2)->Get(String::New("onClose"));
		if (onClose->IsFunction()) {
			std::cout << "step 1\n";
			v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(onClose);
			std::cout << "step 2\n";
			v8::Handle<v8::Value> emptyArgs[] = {};
			std::cout << "step 3\n";
			callback->Call((*data2), 0, emptyArgs);
			std::cout << "step 4 ok!!!!!\n";
		}
	}

    fake_main_iteration(retrans);
  
  g_signal_connect (G_OBJECT(wnd), "destroy", G_CALLBACK(window_destroy), static_cast<void*>(data));
  
  
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


Handle<Value>
button(const Arguments& args)
{
  HandleScope scope;
  Local<Object> buttonObject = Object::New();
  
  GtkWidget *btn = gtk_button_new();
  
  buttonObject->Set(String::New("handle"),  v8::External::New(btn));
  buttonObject->Set(String::New("setTitle"), FunctionTemplate::New(button_setTitle)->GetFunction());
  
  //Local<Object> params = args[0]->ToObject();
  return buttonObject;
}



extern "C" void init (Handle<Object> target)
{
  HandleScope scope;
  gtk_init(0,(char***)"");
  target->Set(String::New("window"), FunctionTemplate::New(window)->GetFunction());
  target->Set(String::New("button"), FunctionTemplate::New(button)->GetFunction());

  target->Set(String::New("main"), FunctionTemplate::New(main_iteration)->GetFunction());
}

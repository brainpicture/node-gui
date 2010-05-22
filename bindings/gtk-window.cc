#include <v8.h>

#include <gtk/gtk.h>
#include <gtk/gtkenums.h>
#include "gtk-node.h"

#include <iostream>
#include <stdio.h>
using namespace v8;

// Window ----------------------------------

Handle<Value> window_show(const Arguments& args) {
    HandleScope scope;
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
    gtk_widget_show_all(wnd);
    
    main_loop_level++;
    if (main_loop_level == 1) {
  	v8::Handle<v8::Value> on_show = windowObject->Get(String::New("on_show"));
	if (on_show->IsFunction()) {
	    v8::Handle<v8::Value> emptyArgs[] = {};
	    v8::Handle<v8::Function>::Cast(on_show)->Call(windowObject, 0, emptyArgs);
	}
    }
  
    return windowObject;
}

Handle<Value> window_set_title(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
    String::Utf8Value title(args[0]->ToString());
    gtk_window_set_title(GTK_WINDOW(wnd), *title);
    return windowObject;
}

Handle<Value> window_set_size(const Arguments& args) {
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

/**
   Set the dimensions of the window frame.
 */
Handle<Value> window_set_frame_dimensions(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
    int left,top,right,bottom; //omg css
    if (args[0]->IsNumber())
  	left=args[0]->ToInteger()->Value();
  
    if (args[1]->IsNumber())
  	top=args[1]->ToInteger()->Value();

    if (args[2]->IsNumber())
  	right=args[2]->ToInteger()->Value();

    if (args[3]->IsNumber())
  	bottom=args[3]->ToInteger()->Value();

    gtk_window_set_frame_dimensions(GTK_WINDOW(wnd),left,top,right,bottom);
    return windowObject;
}

/**
   This method sets the position of the window.

   http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowPosition
*/
Handle<Value> window_set_position(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
    
    GtkWindowPosition pos;
    int val;
    if (args[0]->IsNumber())
	val = args[0]->ToInteger()->Value();
    
    if ((val == NULL) || (val < 0 && val > 4)) {
  	pos = GTK_WIN_POS_CENTER; // just center if not specified or if out of bounds.
    } else {
	if (val == 0)
	    pos = GTK_WIN_POS_NONE;
	if (val == 1)
	    pos = GTK_WIN_POS_CENTER;
	if (val == 2)
	    pos = GTK_WIN_POS_MOUSE;
	if (val == 3)
	    pos = GTK_WIN_POS_CENTER_ALWAYS;
	if (val == 4)
	    pos = GTK_WIN_POS_CENTER_ON_PARENT;
    }

    gtk_window_set_position(GTK_WINDOW(wnd), pos);
    return windowObject;
}

/**
   This method sets the opacity of the window.
*/
Handle<Value> window_set_opacity(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
    
    double opacity;
    if (args[0]->IsNumber())
  	opacity=args[0]->NumberValue();
    
    if (opacity == NULL || (opacity < 0.0 && opacity > 1.0)) // just set it to full if nobody has set anything.
  	opacity = 1.0;

    gtk_window_set_opacity(GTK_WINDOW(wnd), opacity);
    return windowObject;
}

/**
   This method sets the resizability of the window.
*/
Handle<Value> window_set_resizable(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
    
    bool resizable;
    if (args[0]->IsBoolean())
  	resizable=args[0]->ToBoolean()->Value();
    
    if (resizable == NULL)
  	resizable = false;
    
    gtk_window_set_resizable(GTK_WINDOW(wnd), resizable);
    return windowObject;
}

/**
   This method adds items to this window.

   If you are looking to place items within the GTK Window, use this method.
*/
Handle<Value> window_add(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
  
    Local<Object> otherObject = args[0]->ToObject();
    GtkWidget *obj = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(otherObject->Get(String::New("handle")))->Value());
  
    gtk_container_add(GTK_CONTAINER (wnd), obj);
    gtk_widget_show(obj);
    return windowObject;
}

/**
   Allows for callbacks 
*/
void window_on_close_callback(GtkWidget *widget, gpointer dataCast) {
    HandleScope scope; //posible memory leak?
    Persistent<Object> *data = reinterpret_cast<Persistent<Object>*>(dataCast);
    v8::Handle<v8::Value> on_close = (*data)->Get(String::New("on_close"));
    if (on_close->IsFunction()) {
	v8::Handle<v8::Function> callback = v8::Handle<v8::Function>::Cast(on_close);
	v8::Handle<v8::Value> emptyArgs[] = {};
	callback->Call((*data), 0, emptyArgs);
    }
    main_loop_level--;
}

Handle<Value> window_on_close(const Arguments& args) {
    Local<Object> windowObject = args.This();
    GtkWidget *wnd = static_cast<GtkWidget*>(v8::Handle<v8::External>::Cast(windowObject->Get(String::New("handle")))->Value());
  
    windowObject->Set(String::New("on_close"), args[0]);

    Persistent<Object> *data = new Persistent<Object>();
    *data = Persistent<Object>::New(windowObject);
  
    g_signal_connect(G_OBJECT(wnd), "destroy", G_CALLBACK(window_on_close_callback), (gpointer) data);
    return windowObject;
}

Handle<Value> window(const Arguments& args) {
    HandleScope scope;
  
    Local<Object> windowObject = Object::New();
  
    GtkWidget *wnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    Local<Object> params = args[0]->ToObject();
  
  
    windowObject->Set(String::New("handle"),  v8::External::New(wnd));

    windowObject->Set(String::New("show"), FunctionTemplate::New(window_show)->GetFunction());
    windowObject->Set(String::New("set_title"), FunctionTemplate::New(window_set_title)->GetFunction());
    windowObject->Set(String::New("set_size"), FunctionTemplate::New(window_set_size)->GetFunction());
    windowObject->Set(String::New("set_frame_dimensions"), FunctionTemplate::New(window_set_frame_dimensions)->GetFunction());
    windowObject->Set(String::New("set_position"), FunctionTemplate::New(window_set_position)->GetFunction());
    windowObject->Set(String::New("set_opacity"), FunctionTemplate::New(window_set_opacity)->GetFunction());
    windowObject->Set(String::New("set_resizable"), FunctionTemplate::New(window_set_resizable)->GetFunction());
    windowObject->Set(String::New("add"), FunctionTemplate::New(window_add)->GetFunction());
    windowObject->Set(String::New("on_close"), FunctionTemplate::New(window_on_close)->GetFunction());
  
    return windowObject;
}

#include <v8.h>

#include <gtk/gtk.h>
#include <gtk/gtkenums.h>
#include "gtk-node.h"

#include <stdio.h>
#include <stdlib.h>
using namespace v8;

/**
   Show the window.
 */
Handle<Value> WindowShow (const Arguments& args) {
  HandleScope scope;

  Local<Object> windowObject = args.This();
  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, windowObject);

  gtk_widget_show_all(wnd);

  main_loop_level++;

  if (main_loop_level == 1) {
    Handle<Value> onShow = windowObject->Get(String::New("onShow"));

    if (onShow->IsFunction()) {
      v8::Handle<v8::Function>::Cast(onShow)->Call(windowObject, 0, NULL);
    }
  }

  return args.This();
}

/**
   Set the window title
 */
Handle<Value> WindowSetTitle (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  String::Utf8Value title(args[0]->ToString());
  gtk_window_set_title(GTK_WINDOW(wnd), *title);

  return args.This();
}

// Get the window title.
Handle<Value> WindowGetTitle (const Arguments& args) {
  HandleScope scope;

  //GtkWidget *wnd;
  //GTKNODE_GET_HANDLE(wnd, args.This());

  //printf("%s\n", gtk_window_get_title(GTK_WINDOW(wnd)));

  //return scope.Close(String::New(gtk_window_get_title(GTK_WINDOW(wnd))));
  return scope.Close(args.This());
}

/**
   Set the window size
 */
Handle<Value> WindowSetSize (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  int width, height;
  if (args[0]->IsNumber()) {
    width = args[0]->ToInteger()->Value();
  } else {
    width = 640;
  }

  if (args[1]->IsNumber()) {
    height = args[1]->ToInteger()->Value();
  } else {
    height = 480;
  }

  gtk_window_set_default_size(GTK_WINDOW(wnd), width, height);

  return args.This();
}

/**
   Set the dimensions of the window frame.
 */
Handle<Value> WindowSetFrameDimensions (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  int left, top, right, bottom; //omg css

  if (args[0]->IsNumber()) {
    left = args[0]->ToInteger()->Value();
  }

  if (args[1]->IsNumber()) {
    top = args[1]->ToInteger()->Value();
  }

  if (args[2]->IsNumber()) {
    right = args[2]->ToInteger()->Value();
  }

  if (args[3]->IsNumber()) {
    bottom = args[3]->ToInteger()->Value();
  }

  gtk_window_set_frame_dimensions(GTK_WINDOW(wnd), left, top, right, bottom);

  return args.This();
}

/**
   This method sets the position of the window.

   http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GtkWindowPosition
*/
Handle<Value> WindowSetPosition (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  GtkWindowPosition pos;
  int val;

  if (args[0]->IsNumber()) {
    val = args[0]->ToInteger()->Value();
  }

  switch (val) {
    case 0:
      pos = GTK_WIN_POS_NONE;
      break;
    case 1:
      pos = GTK_WIN_POS_CENTER;
      break;
    case 2:
      pos = GTK_WIN_POS_MOUSE;
      break;
    case 3:
      pos = GTK_WIN_POS_CENTER_ALWAYS;
      break;
    case 4:
      pos = GTK_WIN_POS_CENTER_ON_PARENT;
      break;
    default:
      pos = GTK_WIN_POS_CENTER;
      break;
  }

  gtk_window_set_position(GTK_WINDOW(wnd), pos);

  return args.This();
}

// gtk_window_get_position(GtkWindow, gint x, gint y)
// returns void
Handle<Value> WindowGetPosition (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  gint *x = 0;
  gint *y = 0;

  gtk_window_get_position(GTK_WINDOW(wnd), x, y);

  Local<Array> ret = Array::New(2);
  ret->Set(0, Integer::New(*x));
  ret->Set(1, Integer::New(*y));

  return scope.Close(ret);
}

/**
   This method sets the opacity of the window.
*/
Handle<Value> WindowSetOpacity (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  double opacity;
  if (args[0]->IsNumber()) {
    opacity = args[0]->NumberValue();

    if (opacity < 0.0 && opacity > 1.0) { // just set it to full if nobody has set anything.
      opacity = 1.0;
    }
  } else {
    opacity = 1.0;
  }

  gtk_window_set_opacity(GTK_WINDOW(wnd), opacity);

  return args.This();
}

/**
   This method sets the resizability of the window.
*/
Handle<Value> WindowSetResizable (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  bool resizable;
  if (args[0]->IsBoolean()) {
    resizable = args[0]->ToBoolean()->Value();
  } else {
    resizable = false;
  }

  gtk_window_set_resizable(GTK_WINDOW(wnd), resizable);

  return args.This();
}

// gtk_window_get_resizable(GtkWindow)
// returns gboolean
Handle<Value> WindowGetResizable (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  return scope.Close(Boolean::New(
        gtk_window_get_resizable(GTK_WINDOW(wnd))));
}

/**
   This method adds items to this window.

   If you are looking to place items within the GTK Window, use this method.
*/
Handle<Value> WindowAdd (const Arguments& args) {
  HandleScope scope;

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, args.This());

  GtkWidget *obj;
  GTKNODE_GET_HANDLE(obj, args[0]->ToObject());

  gtk_container_add(GTK_CONTAINER(wnd), obj);
  gtk_widget_show(obj);

  return args.This();
}

/**
   Fire the onClose callback
*/
void windowOnCloseCallback (GtkWidget *widget, gpointer dataCast) {
  Persistent<Object>   *data    = reinterpret_cast<Persistent<Object>*>(dataCast);
  v8::Handle<v8::Value> onClose = (*data)->Get(String::New("onClose"));

  if (onClose->IsFunction()) {
    Handle<Function>::Cast(onClose)->Call(*data, 0, NULL);
  }

  main_loop_level--;
}

/**
   Allow for callbacks when the window closes.
 */
Handle<Value> WindowOnClose (const Arguments& args) {
  HandleScope scope;
  Local<Object> windowObject = args.This();

  GtkWidget *wnd;
  GTKNODE_GET_HANDLE(wnd, windowObject);

  windowObject->Set(String::New("onClose"), args[0]);

  Persistent<Object> *data = new Persistent<Object>();
  *data = Persistent<Object>::New(windowObject);

  g_signal_connect(G_OBJECT(wnd), "destroy", G_CALLBACK(windowOnCloseCallback), (gpointer) data);

  return args.This();
}

/**
   Base window object. This provides JS level method access and returns a window object.
 */
Handle<Value> Window (const Arguments& args) {
  HandleScope scope;

  Local<Object> windowObject = Object::New();

  GtkWidget *wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  windowObject->Set(String::New("handle"),             v8::External::New(wnd));

  windowObject->Set(String::New("show"),               FunctionTemplate::New(WindowShow)->GetFunction());
  windowObject->Set(String::New("setTitle"),           FunctionTemplate::New(WindowSetTitle)->GetFunction());
  windowObject->Set(String::New("getTitle"),           FunctionTemplate::New(WindowGetTitle)->GetFunction());
  windowObject->Set(String::New("setSize"),            FunctionTemplate::New(WindowSetSize)->GetFunction());
  windowObject->Set(String::New("setFrameDimensions"), FunctionTemplate::New(WindowSetFrameDimensions)->GetFunction());
  windowObject->Set(String::New("setPosition"),        FunctionTemplate::New(WindowSetPosition)->GetFunction());
  windowObject->Set(String::New("getPosition"),        FunctionTemplate::New(WindowGetPosition)->GetFunction());
  windowObject->Set(String::New("setOpacity"),         FunctionTemplate::New(WindowSetOpacity)->GetFunction());
  windowObject->Set(String::New("setResizable"),       FunctionTemplate::New(WindowSetResizable)->GetFunction());
  windowObject->Set(String::New("getResizable"),       FunctionTemplate::New(WindowGetResizable)->GetFunction());
  windowObject->Set(String::New("add"),                FunctionTemplate::New(WindowAdd)->GetFunction());
  windowObject->Set(String::New("onClose"),            FunctionTemplate::New(WindowOnClose)->GetFunction());

  return scope.Close(windowObject);
}

#include "ngtk_widget.h"
#include <node.h>
#include <string.h>

namespace ngtk {

using namespace v8;
using namespace node;

// Add signal handler.
Handle<Value> Widget::On (const Arguments &args) {
  HandleScope scope;
  guint id = 0;

  if (args[0]->IsString() && args[1]->IsFunction()) {
    GtkWidget *widget = ObjectWrap::Unwrap<Widget>(args.This())->widget_;
    GObject *object = G_OBJECT(widget);

    Persistent<Function> *callback = new Persistent<Function>();
    *callback = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
    gpointer callback_ptr = (gpointer) callback;

    for (std::vector<SignalCallback>::iterator it = Widget::callbacks->begin(); it != Widget::callbacks->end(); ++it) {
      if (strcmp(it->name, *String::AsciiValue(args[0]->ToString())) == 0) {
        id = g_signal_connect(object, it->name, it->callback, callback_ptr);
        break;
      }
    }

    return scope.Close(v8::Number::New(id));
  }

  return scope.Close(v8::Boolean::New(false));
}

Handle<Value> Widget::Disconnect (const Arguments &args) {
  if (args[0]->IsNumber()) {

    GtkWidget *instance = Widget::Data(args.This());
    int id = (int)(args[0]->NumberValue());

    if (g_signal_handler_is_connected (instance, id)) {
      g_signal_handler_disconnect (instance, id);
    }
  }

  return args.This();
}


// Show()
// For showing the widget.
Handle<Value> Widget::Show (const Arguments &args) {
  HandleScope scope;

  GtkWidget *widget = ObjectWrap::Unwrap<Widget>(args.This())->widget_;

  gtk_widget_show_all(widget);

  return args.This();
}

// For destroying widgets
Handle<Value> Widget::Destroy (const Arguments &args) {
  HandleScope scope;
  GtkWidget *widget = Widget::Data(args.This());
  gtk_widget_destroy(widget);
  return args.This();
}

// For checking focus
Handle<Value> Widget::IsFocus    (const v8::Arguments &args) {
  HandleScope scope;
  GtkWidget *widget = Widget::Data(args.This());
  return scope.Close(v8::Boolean::New(gtk_widget_is_focus(widget)));
}

// For grabbing focus
Handle<Value> Widget::GrabFocus  (const v8::Arguments &args) {
  HandleScope scope;
  GtkWidget *widget = ObjectWrap::Unwrap<Widget>(args.This())->widget_;
  gtk_widget_grab_focus(widget);
  return args.This();
}


/**
 * Bare signal. Used as the intermediate callback for various signals.
 * Register a focus listener using @widget.on(signal, callback);@. Many signals
 * are bare, this function handles all of them.
 *
 * Parameter:
 *   widget - The widget that triggered the signal
 *   callback_ptr - The user data, which is the JavaScript function to call
 */
void Widget::SignalBare(GtkWidget *widget, gpointer callback_ptr) {
  Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(callback_ptr);
  TryCatch try_catch;

  (*callback)->Call(Context::GetCurrent()->Global(), 0, NULL);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }
}


/**
 * Boolean signal. Used as the intermediate callback for various signals.
 * Register a focus listener using @widget.on(signal, callback);@. Many signals
 * are bare, but return a gboolean to indicate success/failure, or if the event
 * should propagate. This function handles all of them.
 *
 * Parameter:
 *   widget - The widget that triggered the signal
 *   callback_ptr - The user data, which is the JavaScript function to call
 *
 * Returns:
 * True or false, what ever the callback returns
 */
gboolean Widget::SignalBoolean(GtkWidget *widget, gpointer callback_ptr) {
  Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(callback_ptr);
  TryCatch try_catch;

  Local<Value> ret = (*callback)->Call(Context::GetCurrent()->Global(), 0, NULL);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  if (ret.IsEmpty()) {
    printf("Bad return");
  }

  return (*ret)->IsTrue();
}

/**
 * Focus signal. Used as the intermediate callback for focus signals.
 * Register a focus listener using @widget.on('focus', callback);@
 *
 * Parameter:
 *   widget - The widget that triggered the signal
 *   dir - Tab forward, tab backwards, etc. See <GtkDirectionType> enum
 *   callback_ptr - The user data, which is the JavaScript function to call
 */
gboolean Widget::SignalFocus(GtkWidget *widget, GtkDirectionType dir, gpointer callback_ptr) {
  Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(callback_ptr);
  TryCatch try_catch;

  Local<Value> argv[1] = { v8::Number::New(dir) };

  Local<Value> ret = (*callback)->Call(Context::GetCurrent()->Global(), 1, argv);

  if (try_catch.HasCaught()) {
    FatalException(try_catch);
  }

  if (ret.IsEmpty()) {
    printf("Bad return");
  }

  return (*ret)->IsTrue();
}


/**
 * Bind methods on the JavaScript object to functions on the Widget class
 */
void Widget::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "on",         Widget::On);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "disconnect", Widget::Disconnect);

  // Maybe over-riden by Window for ev_ref etc.
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "show",       Widget::Show);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "destroy",    Widget::Destroy);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "isFocus",    Widget::IsFocus);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "grabFocus",  Widget::GrabFocus);
}

void Widget::RegisterCallbacks (std::vector<SignalCallback> *callbacks) {
  (*callbacks).push_back(SignalCallback("focus", G_CALLBACK(Widget::SignalFocus)));

  (*callbacks).push_back(SignalCallback("grab-focus", G_CALLBACK(Widget::SignalBare)));
  (*callbacks).push_back(SignalCallback("hide", G_CALLBACK(Widget::SignalBare)));
  (*callbacks).push_back(SignalCallback("map", G_CALLBACK(Widget::SignalBare)));
  (*callbacks).push_back(SignalCallback("show", G_CALLBACK(Widget::SignalBare)));
  (*callbacks).push_back(SignalCallback("realize", G_CALLBACK(Widget::SignalBare)));
  (*callbacks).push_back(SignalCallback("unmap", G_CALLBACK(Widget::SignalBare)));
  (*callbacks).push_back(SignalCallback("unrealize", G_CALLBACK(Widget::SignalBare)));

  (*callbacks).push_back(SignalCallback("popup-menu", G_CALLBACK(Widget::SignalBoolean)));
  (*callbacks).push_back(SignalCallback("destroy", G_CALLBACK(Widget::SignalBoolean)));
}

std::vector<SignalCallback> *Widget::callbacks = new std::vector<SignalCallback>;

/*
void Widget::SignalAccelClosuresChanged(GtkWidget *widget, gpointer callback_ptr) {
  Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(callback_ptr);
}


gboolean Widget::SignalButtonPressEvent(GtkWidget *widget, GdkEventButton *event, gpointer callback_ptr);
gboolean Widget::SignalButtonReleaseEvent(GtkWidget *widget, GdkEventButton *event, gpointer callback_ptr);
gboolean Widget::SignalCanActivateAccel(GtkWidget *widget, guint signal_id, gpointer callback_ptr);
void Widget::SignalChildNotify(GtkWidget *widget, GParamSpec *pspec, gpointer callback_ptr);
gboolean Widget::SignalClientEvent(GtkWidget *widget, GdkEventClient *event, gpointer callback_ptr);
gboolean Widget::SignalConfigureEvent(GtkWidget *widget, GdkEventConfigure *event, gpointer callback_ptr);
gboolean Widget::SignalDeleteEvent(GtkWidget *widget, GdkEvent *event, gpointer callback_ptr);
gboolean Widget::SignalDestroyEvent(GtkWidget *widget, GdkEvent *event, gpointer callback_ptr);
void Widget::SignalDirectionChanged(GtkWidget *widget, GtkTextDirection arg1, gpointer callback_ptr);
void Widget::SignalDragBegin(GtkWidget *widget, GdkDragContext *drag_context, gpointer callback_ptr);
void Widget::SignalDragDataDelete(GtkWidget *widget, GdkDragContext *drag_context, gpointer callback_ptr);
void Widget::SignalDragDataGet(GtkWidget *widget, GdkDragContext *drag_context, GtkSelectionData *data, guint info, guint time, gpointer callback_ptr);
void Widget::SignalDragDataReceived(GtkWidget *widget, GdkDragContext *drag_context, gint x, gint y, GtkSelectionData *data, guint info, guint time, gpointer callback_ptr);
gboolean Widget::SignalDragDrop(GtkWidget *widget, GdkDragContext *drag_context, gint x, gint y, guint time, gpointer callback_ptr);
void Widget::SignalDragEnd(GtkWidget *widget, GdkDragContext *drag_context, gpointer callback_ptr);
void Widget::SignalDragLeave(GtkWidget *widget, GdkDragContext *drag_context, guint time, gpointer callback_ptr);
gboolean Widget::SignalDragMotion(GtkWidget *widget, GdkDragContext *drag_context, gint x, gint y, guint time, gpointer callback_ptr);
gboolean Widget::SignalEnterNotifyEvent(GtkWidget *widget, GdkEventCrossing *event, gpointer callback_ptr);
gboolean Widget::SignalEvent(GtkWidget *widget, GdkEvent *event, gpointer callback_ptr);
void Widget::SignalEventAfter(GtkWidget *widget, GdkEvent *event, gpointer callback_ptr);
gboolean Widget::SignalExposeEvent(GtkWidget *widget, GdkEventExpose *event, gpointer callback_ptr);
gboolean Widget::SignalFocusInEvent(GtkWidget *widget, GdkEventFocus *event, gpointer callback_ptr);
gboolean Widget::SignalFocusOutEvent(GtkWidget *widget, GdkEventFocus *event, gpointer callback_ptr);
void Widget::SignalGrabNotify(GtkWidget *widget, gboolean arg1, gpointer callback_ptr);
void Widget::SignalHierarchyChanged(GtkWidget *widget, GtkWidget *widget2, gpointer callback_ptr);
gboolean Widget::SignalKeyPressEvent(GtkWidget *widget, GdkEventKey *event, gpointer callback_ptr);
gboolean Widget::SignalKeyReleaseEvent(GtkWidget *widget, GdkEventKey *event, gpointer callback_ptr);
gboolean Widget::SignalLeaveNotifyEvent(GtkWidget *widget, GdkEventCrossing *event, gpointer callback_ptr);
gboolean Widget::SignalMapEvent(GtkWidget *widget, GdkEvent *event, gpointer callback_ptr);
gboolean Widget::SignalMnemonicActivate(GtkWidget *widget, gboolean arg1, gpointer callback_ptr);
gboolean Widget::SignalMotionNotifyEvent(GtkWidget *widget, GdkEventMotion *event, gpointer callback_ptr);
gboolean Widget::SignalNoExposeEvent(GtkWidget *widget, GdkEventNoExpose *event, gpointer callback_ptr);
void Widget::SignalParentSet(GtkWidget *widget, GtkObject *old_parent, gpointer callback_ptr);
gboolean Widget::SignalPropertyNotifyEvent(GtkWidget *widget, GdkEventProperty *event, gpointer callback_ptr);
gboolean Widget::SignalProximityInEvent(GtkWidget *widget, GdkEventProximity *event, gpointer callback_ptr);
gboolean Widget::SignalProximityOutEvent(GtkWidget *widget, GdkEventProximity *event, gpointer callback_ptr);
void Widget::SignalScreenChanged(GtkWidget *widget, GdkScreen *arg1, gpointer callback_ptr);
gboolean Widget::SignalScrollEvent(GtkWidget *widget, GdkEventScroll *event, gpointer callback_ptr);
gboolean Widget::SignalSelectionClearEvent(GtkWidget *widget, GdkEventSelection *event, gpointer callback_ptr);
void Widget::SignalSelectionGet(GtkWidget *widget, GtkSelectionData *data, guint info, guint time, gpointer callback_ptr);
gboolean Widget::SignalSelectionNotifyEvent(GtkWidget *widget, GdkEventSelection *event, gpointer callback_ptr);
void Widget::SignalSelectionReceived(GtkWidget *widget, GtkSelectionData *data, guint time, gpointer callback_ptr);
gboolean Widget::SignalSelectionRequestEvent(GtkWidget *widget, GdkEventSelection *event, gpointer callback_ptr);
gboolean Widget::SignalShowHelp(GtkWidget *widget, GtkWidgetHelpType arg1, gpointer callback_ptr);
void Widget::SignalSizeAllocate(GtkWidget *widget, GtkAllocation *allocation, gpointer callback_ptr);
void Widget::SignalSizeRequest(GtkWidget *widget, GtkRequisition *requisition, gpointer callback_ptr);
void Widget::SignalStateChanged(GtkWidget *widget, GtkStateType state, gpointer callback_ptr);
void Widget::SignalStyleSet(GtkWidget *widget, GtkStyle *previous_style, gpointer callback_ptr);
gboolean Widget::SignalUnmapEvent(GtkWidget *widget, GdkEvent *event, gpointer callback_ptr);
gboolean Widget::SignalVisibilityNotifyEvent(GtkWidget *widget, GdkEventVisibility *event, gpointer callback_ptr);
gboolean Widget::SignalWindowStateEvent(GtkWidget *widget, GdkEventWindowState *event, gpointer user_data);
*/


} // namespace ngtk

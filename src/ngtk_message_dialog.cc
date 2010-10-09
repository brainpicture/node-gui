#include "ngtk_message_dialog.h"

#include <v8.h>
#include <gtk/gtk.h>
#include "ngtk.h"

#include <stdio.h>

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> MessageDialog::constructor_template;

// Public constructor
MessageDialog* MessageDialog::New (void) {
  HandleScope scope;

  Local<Object> dialog = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<MessageDialog>(dialog);
}

// ECMAScript constructor.
Handle<Value> MessageDialog::New (const Arguments &args) {
  HandleScope scope;

  MessageDialog *window = new MessageDialog();
  window->Wrap(args.This());

  // Add the onDestroy handler
  Persistent<Object> *self = new Persistent<Object>();
  *self = Persistent<Object>::New(args.This());

  g_signal_connect(G_OBJECT(window->widget_), "destroy", G_CALLBACK(MessageDialog::onDestroy), (gpointer) self);

  return args.This();
}

MessageDialog::MessageDialog (GtkWindow *parent, GtkDialogFlags flags,
    GtkMessageType type, GtkButtonsType buttons, const gchar *message_format) {
  widget_ = gtk_dialog_new(parent, flags, type, buttons, message_format);
}

// Show()
// For showing the window.
Handle<Value> MessageDialog::Show (const Arguments &args) {
  HandleScope scope;

  GtkWidget *window = Widget::Gtk(args.This());

  gtk_widget_show_all(window);

  main_loop_level++;

  if (main_loop_level == 1) {
    Handle<Value> onShow = args.This()->Get(String::New("onShow"));

    if (onShow->IsFunction()) {
      v8::Handle<v8::Function>::Cast(onShow)->Call(args.This(), 0, NULL);
    }
  }

  return args.This();
}

// Export.
void MessageDialog::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(MessageDialog::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("MessageDialog"));

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "show", MessageDialog::Show);

  target->Set(String::NewSymbol("MessageDialog"), constructor_template->GetFunction());
}

} // namespace ngtk


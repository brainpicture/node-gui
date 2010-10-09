#include "ngtk_message_dialog.h"

#include <v8.h>
#include <gtk/gtk.h>
#include "ngtk.h"

#include <stdio.h>

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> MessageDialog::constructor_template;

// Public constructor
MessageDialog* MessageDialog::New (Window *parent, GtkDialogFlags flags,
    GtkMessageType type, GtkButtonsType buttons, gchar *message) {
  HandleScope scope;

  Local<Value> argv[5];
  argv[0] = *parent->handle_;
  argv[1] = Number::New(flags);
  argv[2] = Number::New(type);
  argv[3] = Number::New(buttons);
  argv[4] = String::New(message);

  Local<Object> dialog = constructor_template->GetFunction()->NewInstance(5, argv);

  return ObjectWrap::Unwrap<MessageDialog>(dialog);
}

// ECMAScript constructor.
Handle<Value> MessageDialog::New (const Arguments &args) {
  HandleScope scope;

  // TODO: Type checking.

  MessageDialog *dialog = new MessageDialog(NULL,
      (GtkDialogFlags) args[1]->Int32Value(), (GtkMessageType) args[2]->Int32Value(),
      (GtkButtonsType) args[3]->Int32Value(), *String::Utf8Value(args[4]->ToString()));

  dialog->Wrap(args.This());

  // Add the onDestroy handler
  Persistent<Object> *self = new Persistent<Object>();
  *self = Persistent<Object>::New(args.This());

  g_signal_connect(G_OBJECT(dialog->widget_), "destroy", G_CALLBACK(MessageDialog::onDestroy), (gpointer) self);

  return args.This();
}

MessageDialog::MessageDialog (GtkWindow *parent, GtkDialogFlags flags,
    GtkMessageType type, GtkButtonsType buttons, char *message) {
  widget_ = gtk_message_dialog_new(parent, flags, type, buttons, "%s", message);
}

// Run()
// For showing the dialog.
Handle<Value> MessageDialog::Run (const Arguments &args) {
  HandleScope scope;

  GtkWidget *dialog = Widget::Gtk(args.This());

  gtk_dialog_run(GTK_DIALOG(dialog));

  Handle<Value> onRun = args.This()->Get(String::New("onRun"));

  if (onRun->IsFunction()) {
    v8::Handle<v8::Function>::Cast(onRun)->Call(args.This(), 0, NULL);
  }

  return args.This();
}

// Destroy()
// For removing the dialog.
Handle<Value> MessageDialog::Destroy (const Arguments &args) {
  HandleScope scope;

  GtkWidget *dialog = Widget::Gtk(args.This());

  gtk_widget_destroy(dialog);

  return args.This();
}

// Export.
void MessageDialog::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(MessageDialog::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("MessageDialog"));

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "run",     MessageDialog::Run);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "destroy", MessageDialog::Destroy);

  target->Set(String::NewSymbol("MessageDialog"), constructor_template->GetFunction());
}

} // namespace ngtk


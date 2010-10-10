#include "ngtk_message_dialog.h"

#include <v8.h>
#include <gtk/gtk.h>
#include "ngtk.h"

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

  return args.This();
}

MessageDialog::MessageDialog (GtkWindow *parent, GtkDialogFlags flags,
    GtkMessageType type, GtkButtonsType buttons, char *message) {
  widget_ = gtk_message_dialog_new(parent, flags, type, buttons, "%s", message);
}

// Export.
void MessageDialog::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(MessageDialog::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("MessageDialog"));

  Widget::Initialize(constructor_template);

  target->Set(String::NewSymbol("MessageDialog"), constructor_template->GetFunction());
}

} // namespace ngtk


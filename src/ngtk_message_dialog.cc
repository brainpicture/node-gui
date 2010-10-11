#include "ngtk_message_dialog.h"

#include <assert.h>
#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_window.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> MessageDialog::constructor_template;

// Public constructor
MessageDialog* MessageDialog::New (Window *parent, GtkDialogFlags flags,
    GtkMessageType type, GtkButtonsType buttons, char *message) {
  HandleScope scope;

  Local<Value> argv[5];
  argv[0] = *parent->handle_;
  argv[1] = Integer::New(flags);
  argv[2] = Integer::New(type);
  argv[3] = Integer::New(buttons);
  argv[4] = String::New(message);

  Local<Object> dialog = constructor_template->GetFunction()->NewInstance(5, argv);

  return ObjectWrap::Unwrap<MessageDialog>(dialog);
}

// ECMAScript constructor.
Handle<Value> MessageDialog::New (const Arguments &args) {
  HandleScope scope;

  if (4 > args.Length()) {
    return ThrowException(Exception::Error(
          String::New("Expects arguments: window or null, dialog flags, message type, button flags[, message]")));
  }

  GtkWindow *parent;

  if (Window::HasInstance(args[0])) {
    parent = GTK_WINDOW(Window::Data(args[0]->ToObject()));
  } else {
    parent = NULL;
  }

  MessageDialog *dialog = new MessageDialog(parent,
      (GtkDialogFlags) args[1]->Int32Value(), (GtkMessageType) args[2]->Int32Value(),
      (GtkButtonsType) args[3]->Int32Value(), *String::Utf8Value(args[4]->ToString()));

  dialog->Wrap(args.This());

  return args.This();
}

MessageDialog::MessageDialog (GtkWindow *parent, GtkDialogFlags flags,
    GtkMessageType type, GtkButtonsType buttons, char *message) {
  widget_ = gtk_message_dialog_new(parent, flags, type, buttons, "%s", message);
}

// Check whether is an instance.
bool MessageDialog::HasInstance (v8::Handle<v8::Value> val) {
  HandleScope scope;

  if (val->IsObject()) {
    v8::Local<v8::Object> obj = val->ToObject();

    if (constructor_template->HasInstance(obj)) {
      return true;
    }
  }

  return false;
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


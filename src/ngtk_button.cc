#include "ngtk_button.h"
#include "ngtk_image.h"
#include "ngtk.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Button::constructor_template;

// Check whether is an instance.
bool Button::HasInstance (Handle<Value> val) {
  HandleScope scope;

  if (val->IsObject()) {
    Local<Object> obj = val->ToObject();

    if (constructor_template->HasInstance(obj)) {
      return true;
    }
  }

  return false;
}

// Public constructor
Button* Button::New (void) {
  HandleScope scope;

  Local<Object> button = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<Button>(button);
}

// ECMAScript constructor.
Handle<Value> Button::New (const Arguments &args) {
  HandleScope scope;

  Button *button = new Button();
  button->Wrap(args.This());

  return args.This();
}

Button::Button (void) {
  widget_ = gtk_button_new();
}

// SetLabel()
// For setting the button label.
Handle<Value> Button::SetLabel (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *button = Button::Data(args.This());

    gtk_button_set_label(GTK_BUTTON(button), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// GetLabel()
// For getting the button label.
Handle<Value> Button::GetLabel (const Arguments &args) {
  HandleScope scope;

  GtkWidget *button = Button::Data(args.This());

  return scope.Close(String::New(gtk_button_get_label(GTK_BUTTON(button))));
}

// SetImage()
// For setting the button image.
Handle<Value> Button::SetImage (const Arguments &args) {
  HandleScope scope;

  GtkWidget *button = Button::Data(args.This());
  GtkWidget *other = Image::Data(args[0]->ToObject());

  gtk_button_set_image(GTK_BUTTON(button), other);
  
  return args.This();
}

// Export.
void Button::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Widget::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setLabel", Button::SetLabel);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getLabel", Button::GetLabel);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setImage", Button::SetImage);
}

void Button::RegisterCallbacks (std::vector<SignalCallback> *callbacks) {
  Widget::RegisterCallbacks(callbacks);
  (*callbacks).push_back(SignalCallback("clicked", G_CALLBACK(Widget::SignalBare)));
}

void Button::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Button::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Button"));

  Button::SetPrototypeMethods(constructor_template);

  //Button::callbacks = new std::vector<SignalCallback>;
  Button::RegisterCallbacks(Button::callbacks);

  target->Set(String::NewSymbol("Button"), constructor_template->GetFunction());
}

} // namespace ngtk


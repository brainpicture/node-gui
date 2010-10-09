#include "ngtk_button.h"

#include <v8.h>
#include <gtk/gtk.h>
#include "ngtk.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Button::constructor_template;

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

  g_signal_connect(G_OBJECT(button->widget_), "destroy", G_CALLBACK(Button::onDestroy),
      (gpointer) &button->handle_);
  g_signal_connect(G_OBJECT(button->widget_), "clicked", G_CALLBACK(Button::onClick),
      (gpointer) &button->handle_);

  return args.This();
}

Button::Button (void) {
  widget_ = gtk_button_new();
}

// onClick() - called on a click event.
void Button::onClick (GtkWidget *button, gpointer dataCast) {
  v8::Persistent<v8::Object> *self    = reinterpret_cast<v8::Persistent<v8::Object>*>(dataCast);
  v8::Handle<v8::Value>       onClick = (*self)->Get(v8::String::New("onClick"));

  if (onClick->IsFunction()) {
    v8::Handle<v8::Function>::Cast(onClick)->Call(*self, 0, NULL);
  }
}

// SetLabel()
// For setting the button label.
Handle<Value> Button::SetLabel (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *button = Widget::Gtk(args.This());

    gtk_button_set_label(GTK_BUTTON(button), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// GetLabel()
// For getting the button label.
Handle<Value> Button::GetLabel (const Arguments &args) {
  HandleScope scope;

  GtkWidget *button = Widget::Gtk(args.This());

  return scope.Close(String::New(gtk_button_get_label(GTK_BUTTON(button))));
}

// Export.
void Button::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Button::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Button"));

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setLabel", Button::SetLabel);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getLabel", Button::GetLabel);

  target->Set(String::NewSymbol("Button"), constructor_template->GetFunction());
}

} // namespace ngtk


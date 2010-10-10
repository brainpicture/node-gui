#include "ngtk_entry.h"

#include <assert.h>
#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Entry::constructor_template;

// Public constructor
Entry* Entry::New (void) {
  HandleScope scope;

  Local<Object> dialog = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<Entry>(dialog);
}

// ECMAScript constructor.
Handle<Value> Entry::New (const Arguments &args) {
  HandleScope scope;

  Entry *entry = new Entry();
  entry->Wrap(args.This());

  return args.This();
}

Entry::Entry (void) {
  widget_ = gtk_entry_new();
}

// gtk_entry_set_text()
Handle<Value> Entry::SetText (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_set_text(GTK_ENTRY(entry), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// gtk_entry_get_text()
Handle<Value> Entry::GetText (const Arguments &args) {
  HandleScope scope;

  GtkWidget *entry = Entry::Data(args.This());

  return scope.Close(String::New(gtk_entry_get_text(GTK_ENTRY(entry))));
}

// gtk_entry_append_text()
Handle<Value> Entry::AppendText (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_append_text(GTK_ENTRY(entry), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// gtk_entry_prepend_text()
Handle<Value> Entry::PrependText (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_prepend_text(GTK_ENTRY(entry), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// gtk_entry_get_text_length()
Handle<Value> Entry::GetTextLength (const Arguments &args) {
  HandleScope scope;

  GtkWidget *entry = Entry::Data(args.This());

  return scope.Close(Number::New(gtk_entry_get_text_length(GTK_ENTRY(entry))));
}

// gtk_entry_select_region()
Handle<Value> Entry::SelectRegion (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsNumber() && args[1]->IsNumber()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_select_region(GTK_ENTRY(entry), args[0]->Int32Value(),
        args[1]->Int32Value());
  }

  return args.This();
}

// gtk_entry_set_visibility()
Handle<Value> Entry::SetVisibility (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsBoolean()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_set_visibility(GTK_ENTRY(entry), args[0]->ToBoolean()->Value());
  }

  return args.This();
}

// gtk_entry_get_visibility()
Handle<Value> Entry::GetVisibility (const Arguments &args) {
  HandleScope scope;

  GtkWidget *entry = Entry::Data(args.This());

  return scope.Close(Boolean::New(gtk_entry_get_visibility(GTK_ENTRY(entry))));
}

// gtk_editable_set_editable()
Handle<Value> Entry::SetEditable (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsBoolean()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_editable_set_editable(GTK_EDITABLE(entry), args[0]->ToBoolean()->Value());
  }

  return args.This();
}

// gtk_editable_get_editable()
Handle<Value> Entry::GetEditable (const Arguments &args) {
  HandleScope scope;

  GtkWidget *entry = Entry::Data(args.This());

  return scope.Close(Boolean::New(gtk_editable_get_editable(GTK_EDITABLE(entry))));
}

// gtk_entry_set_max_length()
Handle<Value> Entry::SetMaxLength (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsNumber()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_set_max_length(GTK_ENTRY(entry), args[0]->Int32Value());
  }

  return args.This();
}

// gtk_entry_get_max_length()
Handle<Value> Entry::GetMaxLength (const Arguments &args) {
  HandleScope scope;

  GtkWidget *entry = Entry::Data(args.This());

  return scope.Close(Number::New(gtk_entry_get_max_length(GTK_ENTRY(entry))));
}

// gtk_entry_set_alignment()
Handle<Value> Entry::SetAlignment (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsNumber()) {
    GtkWidget *entry = Entry::Data(args.This());

    gtk_entry_set_alignment(GTK_ENTRY(entry), args[0]->NumberValue());
  }

  return args.This();
}

// gtk_entry_get_alignment()
Handle<Value> Entry::GetAlignment (const Arguments &args) {
  HandleScope scope;

  GtkWidget *entry = Entry::Data(args.This());

  return scope.Close(Number::New(gtk_entry_get_alignment(GTK_ENTRY(entry))));
}

// Export.
void Entry::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Entry::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Entry"));

  Widget::Initialize(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setText",       Entry::SetText);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getText",       Entry::GetText);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "appendText",    Entry::AppendText);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "prependText",   Entry::PrependText);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getTextLength", Entry::GetTextLength);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "selectRegion",  Entry::SelectRegion);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setVisibility", Entry::SetVisibility);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getVisibility", Entry::GetVisibility);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setEditable",   Entry::SetEditable);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getEditable",   Entry::GetEditable);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setMaxLength",  Entry::SetMaxLength);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getMaxLength",  Entry::GetMaxLength);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setAlignment",  Entry::SetAlignment);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getAlignment",  Entry::GetAlignment);

  target->Set(String::NewSymbol("Entry"), constructor_template->GetFunction());
}

} // namespace ngtk


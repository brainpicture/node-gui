#include "ngtk_hbox.h"

#include <v8.h>
#include <gtk/gtk.h>
#include "ngtk.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Hbox::constructor_template;

// Public constructor
Hbox* Hbox::New (bool homogeneous, int spacing) {
  HandleScope scope;

  Local<Value> argv[2];
  argv[0] = Local<Value>::New(Boolean::New(homogeneous));
  argv[1] = Integer::New(spacing);

  Local<Object> hbox = constructor_template->GetFunction()->NewInstance(2, argv);

  return ObjectWrap::Unwrap<Hbox>(hbox);
}

// ECMAScript constructor.
Handle<Value> Hbox::New (const Arguments &args) {
  HandleScope scope;

  gboolean homogeneous;
  gint     spacing;

  // homogeneous
  if (args[0]->IsBoolean()) {
    homogeneous = args[0]->ToBoolean()->Value();
  } else {
    homogeneous = false;
  }

  // spacing
  if (args[1]->IsNumber()) {
    spacing = args[1]->Int32Value();
  } else {
    spacing = 0;
  }

  Hbox *hbox = new Hbox(homogeneous, spacing);
  hbox->Wrap(args.This());

  return args.This();
}

Hbox::Hbox (gboolean homogeneous, gint spacing) {
  widget_ = gtk_hbox_new(homogeneous, spacing);
}

// Check whether is an instance.
bool Hbox::HasInstance (v8::Handle<v8::Value> val) {
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
void Hbox::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Hbox::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Hbox"));

  Container::Initialize(constructor_template);

  target->Set(String::NewSymbol("Hbox"), constructor_template->GetFunction());
}

} // namespace ngtk


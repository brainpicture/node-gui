#include "ngtk_vbox.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Vbox::constructor_template;

// Check whether is an instance.
bool Vbox::HasInstance (Handle<Value> val) {
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
Vbox* Vbox::New (bool homogeneous, int spacing) {
  HandleScope scope;

  Local<Value> argv[2];
  argv[0] = Local<Value>::New(Boolean::New(homogeneous));
  argv[1] = Integer::New(spacing);

  Local<Object> vbox = constructor_template->GetFunction()->NewInstance(2, argv);

  return ObjectWrap::Unwrap<Vbox>(vbox);
}

// ECMAScript constructor.
Handle<Value> Vbox::New (const Arguments &args) {
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

  Vbox *vbox = new Vbox(homogeneous, spacing);
  vbox->Wrap(args.This());

  return args.This();
}

Vbox::Vbox (gboolean homogeneous, gint spacing) {
  widget_ = gtk_vbox_new(homogeneous, spacing);
}

// Export.
void Vbox::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Box::SetPrototypeMethods(constructor_template);
}

void Vbox::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Vbox::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Vbox"));

  Vbox::SetPrototypeMethods(constructor_template);

  target->Set(String::NewSymbol("Vbox"), constructor_template->GetFunction());
}

} // namespace ngtk


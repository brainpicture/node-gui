#include "ngtk_image.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Image::constructor_template;

// Check whether is an instance.
bool Image::HasInstance (Handle<Value> val) {
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
Image* Image::New (void) {
  HandleScope scope;

  Local<Object> image = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<Image>(image);
}

// ECMAScript constructor.
Handle<Value> Image::New (const Arguments &args) {
  HandleScope scope;

  Image *image = new Image();
  image->Wrap(args.This());

  return args.This();
}

Image::Image (void) {
  widget_ = gtk_image_new();
}

// SetFromFile()
// For setting the image from a file.
Handle<Value> Image::SetFromFile (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *image = Image::Data(args.This());

    gtk_image_set_from_file(GTK_IMAGE(image), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// Export.
void Image::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Widget::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setFromFile", Image::SetFromFile);
}

void Image::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Image::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Image"));

  Image::SetPrototypeMethods(constructor_template);

  target->Set(String::NewSymbol("Image"), constructor_template->GetFunction());
}

} // namespace ngtk


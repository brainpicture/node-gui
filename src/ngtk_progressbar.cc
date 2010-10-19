#include "ngtk_progressbar.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> ProgressBar::constructor_template;

// Check whether is an instance.
bool ProgressBar::HasInstance (Handle<Value> val) {
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
ProgressBar* ProgressBar::New (void) {
  HandleScope scope;

  Local<Object> progressbar = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<ProgressBar>(progressbar);
}

// ECMAScript constructor.
Handle<Value> ProgressBar::New (const Arguments &args) {
  HandleScope scope;

  ProgressBar *progressbar = new ProgressBar();
  progressbar->Wrap(args.This());

  return args.This();
}

ProgressBar::ProgressBar (void) {
  widget_ = gtk_progress_bar_new();
}

// Pulse()
// For setting the progressbar text.
Handle<Value> ProgressBar::Pulse (const Arguments &args) {
  HandleScope scope;

  GtkWidget *progressbar = ProgressBar::Data(args.This());
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar));

  return args.This();
}

// SetText()
// For setting the progressbar text.
Handle<Value> ProgressBar::SetText (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsString()) {
    GtkWidget *progressbar = ProgressBar::Data(args.This());

    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), *String::Utf8Value(args[0]->ToString()));
  }

  return args.This();
}

// GetText()
// For getting the progressbar label.
Handle<Value> ProgressBar::GetText (const Arguments &args) {
  HandleScope scope;

  GtkWidget *progressbar = ProgressBar::Data(args.This());

  return scope.Close(String::New(gtk_progress_bar_get_text(GTK_PROGRESS_BAR(progressbar))));
}

// SetFraction()
// For setting the fraction of the progressbar to fill.
Handle<Value> ProgressBar::SetFraction (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsNumber()) {
      GtkWidget *progressbar = ProgressBar::Data(args.This());
      double fraction = args[0]->NumberValue();
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), fraction);
  }
  return args.This();
}

// GetFraction()
// For getting the fraction of the progressbar that has been filled.
Handle<Value> ProgressBar::GetFraction (const Arguments &args) {
  HandleScope scope;

  GtkWidget *progressbar = ProgressBar::Data(args.This());

  return scope.Close(Number::New(gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar))));
}

// SetFraction()
// For setting the pulse_step of the progressbar to fill.
Handle<Value> ProgressBar::SetPulseStep (const Arguments &args) {
  HandleScope scope;

  if (args[0]->IsNumber()) {
      GtkWidget *progressbar = ProgressBar::Data(args.This());
      double pulse_step = args[0]->NumberValue();
      gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(progressbar), pulse_step);
  }
  return args.This();
}

// GetPulseStep()
// For getting the pulse_step of the progressbar that has been filled.
Handle<Value> ProgressBar::GetPulseStep (const Arguments &args) {
  HandleScope scope;

  GtkWidget *progressbar = ProgressBar::Data(args.This());

  return scope.Close(Number::New(gtk_progress_bar_get_pulse_step(GTK_PROGRESS_BAR(progressbar))));
}

// Export.
void ProgressBar::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Widget::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "pulse", ProgressBar::Pulse);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setText", ProgressBar::SetText);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getText", ProgressBar::GetText);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setFraction", ProgressBar::SetFraction);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getFraction", ProgressBar::GetFraction);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setPulseStep", ProgressBar::SetPulseStep);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getPulseStep", ProgressBar::GetPulseStep);
}

void ProgressBar::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(ProgressBar::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("ProgressBar"));

  ProgressBar::SetPrototypeMethods(constructor_template);

  target->Set(String::NewSymbol("ProgressBar"), constructor_template->GetFunction());
}

} // namespace ngtk


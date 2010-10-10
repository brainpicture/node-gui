#include "ngtk_loop.h"

#include <v8.h>
#include <glib.h>
#include "ngtk.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> Loop::constructor_template;

Loop* Loop::New (void) {
  Local<Object> loop = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<Loop>(loop);
}

Handle<Value> Loop::New (const Arguments &args) {
  HandleScope scope;

  Loop *loop = new Loop();
  loop->Wrap(args.This());

  return args.This();
}

Loop::Loop (void) {
  loop_ = g_main_loop_new(NULL, false);
}

GMainLoop* Loop::Get (Handle<Object> obj) {
  return ObjectWrap::Unwrap<Loop>(obj)->loop_;
}

Handle<Value> Loop::Run (const Arguments &args) {
  HandleScope scope;

  GMainLoop *loop = Loop::Get(args.This());
  g_main_loop_run(loop);
  //loop->run();

  return Undefined();
}

Handle<Value> Loop::Quit (const Arguments &args) {
  HandleScope scope;

  GMainLoop *loop = Loop::Get(args.This());
  g_main_loop_quit(loop);
  //loop->quit();

  return Undefined();
}

Handle<Value> Loop::IsRunning (const Arguments &args) {
  HandleScope scope;

  GMainLoop *loop = Loop::Get(args.This());
  return scope.Close(Boolean::New(g_main_loop_is_running(loop)));
  //return scope.Close(Boolean::New(loop->is_running()));
}

Handle<Value> Loop::Reference (const Arguments &args) {
  HandleScope scope;

  GMainLoop *loop = Loop::Get(args.This());
  //loop->reference();
  g_main_loop_ref(loop);

  return Undefined();
}

Handle<Value> Loop::Unreference (const Arguments &args) {
  HandleScope scope;

  GMainLoop *loop = Loop::Get(args.This());
  //loop->unreference();
  g_main_loop_unref(loop);

  return Undefined();
}

void Loop::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(Loop::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("Loop"));

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "run",         Loop::Run);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "quit",        Loop::Quit);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "isRunning",   Loop::IsRunning);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "reference",   Loop::Reference);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "unreference", Loop::Unreference);

  target->Set(String::NewSymbol("Loop"), constructor_template->GetFunction());
}

} // namespace ngtk

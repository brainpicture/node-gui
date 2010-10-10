#ifndef NGTK_LOOP_H_
#define NGTK_LOOP_H_

#include <v8.h>
#include <glib.h>
#include <node_object_wrap.h>

namespace ngtk {

// The loop.

class Loop : public node::ObjectWrap {
public:
  static void       Initialize (v8::Handle<v8::Object> target);
  static Loop*      New        (void); // public constructor
  static GMainLoop* Get        (v8::Handle<v8::Object> obj);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New            (const v8::Arguments &args);
  static v8::Handle<v8::Value> Run            (const v8::Arguments &args);
  static v8::Handle<v8::Value> Quit           (const v8::Arguments &args);
  static v8::Handle<v8::Value> IsRunning      (const v8::Arguments &args);
  static v8::Handle<v8::Value> Reference      (const v8::Arguments &args);
  static v8::Handle<v8::Value> Unreference    (const v8::Arguments &args);

  Loop(void);

  GMainLoop *loop_;
};

} // namespace ngtk

#endif


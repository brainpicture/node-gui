#ifndef NGTK_BOX_H_
#define NGTK_BOX_H_

#include "ngtk_container.h"

namespace ngtk {

class Box : public Container {
public:
  static void SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);

private:
  static v8::Handle<v8::Value> PackStart      (const v8::Arguments &args);
  static v8::Handle<v8::Value> PackEnd        (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetHomogeneous (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetHomogeneous (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetSpacing     (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetSpacing     (const v8::Arguments &args);
  static v8::Handle<v8::Value> ReorderChild   (const v8::Arguments &args);
};

} // namespace ngtk

#endif

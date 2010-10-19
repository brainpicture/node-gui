#ifndef NGTK_IMAGE_H_
#define NGTK_IMAGE_H_

#include "ngtk_widget.h"

namespace ngtk {

class Image : public Widget {
public:
  static void       SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);
  static void       Initialize          (v8::Handle<v8::Object> target);
  static Image*    New                 (void); // public constructor
  static bool       HasInstance         (v8::Handle<v8::Value> val);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New      (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetFromFile      (const v8::Arguments &args);

  Image(void);
};

} // namespace ngtk

#endif

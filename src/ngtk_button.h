#ifndef NGTK_BUTTON_H_
#define NGTK_BUTTON_H_

#include "ngtk_widget.h"

namespace ngtk {

class Button : public Widget {
public:
  static void       SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);
  static void       RegisterCallbacks   (std::vector<SignalCallback> *callbacks);
  static void       Initialize          (v8::Handle<v8::Object> target);
  static Button*    New                 (void); // public constructor
  static bool       HasInstance         (v8::Handle<v8::Value> val);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New      (const v8::Arguments &args);

  static v8::Handle<v8::Value> SetLabel (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetLabel (const v8::Arguments &args);

  static v8::Handle<v8::Value> SetImage (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetImagePosition (const v8::Arguments &args);

  Button(void);
};

} // namespace ngtk

#endif

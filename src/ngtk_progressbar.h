#ifndef NGTK_PROGRESSBAR_H_
#define NGTK_PROGRESSBAR_H_

#include "ngtk_widget.h"

namespace ngtk {

class ProgressBar : public Widget {
public:
  static void       SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);
  static void       Initialize          (v8::Handle<v8::Object> target);
  static ProgressBar*    New                 (void); // public constructor
  static bool       HasInstance         (v8::Handle<v8::Value> val);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New      (const v8::Arguments &args);
  
  static v8::Handle<v8::Value> Pulse (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetText (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetText (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetFraction (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetFraction (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetPulseStep (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetPulseStep (const v8::Arguments &args);

  ProgressBar(void);
};

} // namespace ngtk

#endif

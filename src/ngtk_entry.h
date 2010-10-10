#ifndef NGTK_ENTRY_H_
#define NGTK_ENTRY_H_

#include <v8.h>
#include <gtk/gtk.h>

#include "ngtk.h"
#include "ngtk_widget.h"

namespace ngtk {

class Entry : public Widget {
public:
  static void       Initialize (v8::Handle<v8::Object> target);
  static Entry* New (void); // public constructor

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New           (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetText       (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetText       (const v8::Arguments &args);
  static v8::Handle<v8::Value> AppendText    (const v8::Arguments &args);
  static v8::Handle<v8::Value> PrependText   (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetTextLength (const v8::Arguments &args);
  static v8::Handle<v8::Value> SelectRegion  (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetVisibility (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetVisibility (const v8::Arguments &args);
  // gtk_editable_{get,set}_editable
  static v8::Handle<v8::Value> SetEditable   (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetEditable   (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetMaxLength  (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetMaxLength  (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetAlignment  (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetAlignment  (const v8::Arguments &args);

  Entry(void);
};

} // namespace ngtk

#endif

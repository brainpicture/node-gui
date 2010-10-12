#include "ngtk_box.h"

namespace ngtk {

using namespace v8;

//gtk_box_pack_start(GtkBox *box, GtkWidget *widget, gboolean expand, gboolean fill, guint padding);
Handle<Value> Box::PackStart (const Arguments &args) {
  HandleScope scope;

  if (!args[0]->IsObject()) {
    return args.This();
  }

  GtkBox    *box    = GTK_BOX(Box::Data(args.This()));
  GtkWidget *widget = Widget::Data(args[0]->ToObject());
  gboolean   expand, fill;
  guint      padding;

  // gboolean expand
  if (args[1]->IsBoolean()) {
    expand = args[1]->ToBoolean()->Value();
  } else {
    expand = true;
  }

  // gboolean fill
  if (args[2]->IsBoolean()) {
    fill = args[2]->ToBoolean()->Value();
  } else {
    fill = true;
  }

  // guint padding
  if (args[3]->IsNumber()) {
    padding = args[3]->Uint32Value();
  } else {
    padding = 0;
  }

  gtk_box_pack_start(box, widget, expand, fill, padding);

  return args.This();
}

//gtk_box_pack_end(GtkBox *box, GtkWidget *widget, gboolean expand, gboolean fill, guint padding);
Handle<Value> Box::PackEnd (const Arguments &args) {
  HandleScope scope;

  if (!args[0]->IsObject()) {
    return args.This();
  }

  GtkBox    *box    = GTK_BOX(Box::Data(args.This()));
  GtkWidget *widget = Widget::Data(args[0]->ToObject());
  gboolean   expand, fill;
  guint      padding;

  // gboolean expand
  if (args[1]->IsBoolean()) {
    expand = args[1]->ToBoolean()->Value();
  } else {
    expand = true;
  }

  // gboolean fill
  if (args[2]->IsBoolean()) {
    fill = args[2]->ToBoolean()->Value();
  } else {
    fill = true;
  }

  // guint padding
  if (args[3]->IsNumber()) {
    padding = args[3]->Uint32Value();
  } else {
    padding = 0;
  }

  gtk_box_pack_end(box, widget, expand, fill, padding);

  return args.This();
}

// gtk_box_get_homogeneous(GtkBox *box);
Handle<Value> Box::GetHomogeneous (const Arguments &args) {
  HandleScope scope;

  GtkBox *box = GTK_BOX(Box::Data(args.This()));

  return scope.Close(Boolean::New(gtk_box_get_homogeneous(box)));
}

// gtk_box_set_homogeneous(GtkBox *box, gboolean homogeneous);
Handle<Value> Box::SetHomogeneous (const Arguments &args) {
  HandleScope scope;

  GtkBox  *box = GTK_BOX(Box::Data(args.This()));
  gboolean homogeneous;

  if (args[0]->IsBoolean()) {
    homogeneous = args[0]->ToBoolean()->Value();
  } else {
    homogeneous = true;
  }

  gtk_box_set_homogeneous(box, homogeneous);

  return args.This();
}

// gtk_box_get_spacing(GtkBox *box);
Handle<Value> Box::GetSpacing (const Arguments &args) {
  HandleScope scope;

  GtkBox *box = GTK_BOX(Box::Data(args.This()));

  return scope.Close(Integer::New(gtk_box_get_spacing(box)));
}

// gtk_box_set_spacing(GtkBox *box, gint spacing);
Handle<Value> Box::SetSpacing (const Arguments &args) {
  HandleScope scope;

  GtkBox *box = GTK_BOX(Box::Data(args.This()));
  gint    spacing;

  if (args[0]->IsNumber()) {
    spacing = args[0]->Int32Value();
  } else {
    spacing = 0;
  }

  gtk_box_set_spacing(box, spacing);

  return args.This();
}

//gtk_box_reorder_child(GtkBox *box, GtkWidget *widget, gint position);
Handle<Value> Box::ReorderChild (const Arguments &args) {
  HandleScope scope;

  if (!args[0]->IsObject()) {
    return args.This();
  }

  GtkBox    *box    = GTK_BOX(Box::Data(args.This()));
  GtkWidget *widget = Widget::Data(args[0]->ToObject());
  gint       position;

  // gint padding
  if (args[1]->IsNumber()) {
    position = args[1]->Int32Value();
  } else {
    position = 0;
  }

  gtk_box_reorder_child(box, widget, position);

  return args.This();
}

// Export.
void Box::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Container::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "packStart",      Box::PackStart);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "packEnd",        Box::PackEnd);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getHomogeneous", Box::GetHomogeneous);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setHomogeneous", Box::SetHomogeneous);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getSpacing",     Box::GetSpacing);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setSpacing",     Box::SetSpacing);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "reorderChild",   Box::ReorderChild);
}

} // namespace ngtk


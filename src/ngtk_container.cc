#include "ngtk_container.h"

namespace ngtk {

using namespace v8;

// gtk_container_add(GtkContainer *container, GtkWidget *widget);
Handle<Value> Container::Add (const Arguments &args) {
  HandleScope scope;

  GtkContainer *container = GTK_CONTAINER(Container::Data(args.This()));
  GtkWidget    *widget    = Widget::Data(args[0]->ToObject());

  gtk_container_add(container, widget);

  return args.This();
}

// gtk_container_remove(GtkContainer *container, GtkWidget *widget);
Handle<Value> Container::Remove (const Arguments &args) {
  HandleScope scope;

  GtkContainer *container = GTK_CONTAINER(Container::Data(args.This()));
  GtkWidget    *widget    = Widget::Data(args[0]->ToObject());

  gtk_container_remove(container, widget);

  return args.This();
}

// Export.
void Container::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Widget::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "add",    Container::Add);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "remove", Container::Remove);
}

} // namespace ngtk


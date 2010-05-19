#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"

#include <iostream>
using namespace v8;

// Alert ----------------------------------

Handle<Value>
Alert(const Arguments& args)
{
  String::Utf8Value title(args[0]->ToString());
  GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s","Alert");
  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", *title);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return Boolean::New(true);
}

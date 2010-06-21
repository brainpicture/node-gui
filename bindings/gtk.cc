#include <v8.h>

#include <gtk/gtk.h>
#include "gtk-node.h"
#include "gtk-window.h"
#include "gtk-alert.h"
#include "gtk-entry.h"
#include "gtk-button.h"
#include "gtk-hbox.h"
#include "gtk-vbox.h"

#include <iostream>
#include <stdio.h>
using namespace v8;

//global var
int main_loop_level = 0;

Handle<Value> main_iteration(const Arguments& args) {
    while (gtk_events_pending()) {
  	gtk_main_iteration();
    }
  
    return Boolean::New(main_loop_level == 0);
}

extern "C" void init (Handle<Object> target) {
    HandleScope scope;
    gtk_init(NULL, NULL);
    target->Set(String::New("window"), FunctionTemplate::New(window)->GetFunction());
    target->Set(String::New("button"), FunctionTemplate::New(button)->GetFunction());
    target->Set(String::New("entry"), FunctionTemplate::New(entry)->GetFunction());
    target->Set(String::New("HBox"), FunctionTemplate::New(HBox)->GetFunction());
    target->Set(String::New("VBox"), FunctionTemplate::New(VBox)->GetFunction());
    target->Set(String::New("alert"), FunctionTemplate::New(Alert)->GetFunction());

    target->Set(String::New("main"), FunctionTemplate::New(main_iteration)->GetFunction());
}

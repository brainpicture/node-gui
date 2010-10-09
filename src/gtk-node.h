#include <v8.h>
using namespace v8;

// For getting handles.
#define GTKNODE_GET_HANDLE(target, wrap) \
target = static_cast<GtkWidget*>(        \
    v8::Handle<v8::External>::Cast(wrap->Get(String::New("handle")))->Value());

//global var
extern int main_loop_level;


#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>

#define MODULE "label"

//extern napi_ref StyleRef;
//void link_style_to_node(napi_env env, YGNodeRef node, napi_value this);



LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

    printf("LABEL NEW\n");
   	GtkWidget* widget = gtk_label_new("Hello guys & gals.");
    
    dsk_wrap_widget(env,widget,this);

    return this;
}

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    

    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
       DSK_CHILDPROP_I32(left,"x"),
       DSK_CHILDPROP_I32(top,"y")
    }));

    return exports;
}


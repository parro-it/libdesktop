#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>

#define MODULE "label"


static void label_finalize(napi_env env, void *finalize_data, void *finalize_hint) {
    GtkWidget* widget = finalize_data;
    YGNodeRef node = g_object_get_data(G_OBJECT( widget),"yoganode");
    YGNodeFree(node);
}

extern napi_ref StyleRef;
void link_style_to_node(napi_env env, YGNodeRef node, napi_value this);




LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

    printf("LABEL NEW\n");
   	GtkWidget* widget = gtk_label_new("Hello guys & gals.");
    
    dsk_wrap_widget(env,widget,this);
    
    //printf("LABEL 222\n");
    //int32_t width=150;
    //int32_t height=50;
    //YGNodeStyleSetWidth(node,width);
    //YGNodeStyleSetHeight(node,height);

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


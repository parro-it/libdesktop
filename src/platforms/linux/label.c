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
    napi_status status = napi_wrap(env, this, widget, label_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          


    printf("LABEL create node\n");
    YGNodeRef node = YGNodeNew();
    g_object_set_data(G_OBJECT(widget),"yoganode", node);
    YGNodeSetContext(node, widget);

    printf("LABEL 222\n");
    int32_t width=150;
    int32_t height=50;
    //gtk_widget_get_preferred_width(child_gtk,&width,NULL);
    //gtk_widget_get_preferred_height(child_gtk,&height,NULL);
    YGNodeStyleSetWidth(node,width);
    YGNodeStyleSetHeight(node,height);

    printf("set dims\n");
    napi_value constructor;
    napi_value style;

    status = napi_get_reference_value(env, StyleRef, &constructor);
    CHECK_STATUS_THROW(status, napi_get_reference_value);                                          
    printf("constructor %p\n",constructor);
    napi_value null;
    napi_get_null(env,&null);
    status = napi_new_instance(env,constructor,2,(napi_value[]){null,this},&style);
    CHECK_STATUS_THROW(status,napi_new_instance);

    napi_set_named_property(env, this, "style", style);
    printf("DONE\n");
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


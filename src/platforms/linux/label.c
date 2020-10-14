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

LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

    printf("LABEL NEW\n");
   	GtkWidget* widget = gtk_label_new("Hello guys & gals.");
    napi_status status = napi_wrap(env, this, widget, label_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    YGNodeRef node = YGNodeNew();
    g_object_set_data(G_OBJECT(widget),"yoganode", node);
    YGNodeSetContext(node, widget);
    int32_t width=150;
    int32_t height=50;
    //gtk_widget_get_preferred_width(child_gtk,&width,NULL);
    //gtk_widget_get_preferred_height(child_gtk,&height,NULL);
    YGNodeStyleSetWidth(node,width);
    YGNodeStyleSetHeight(node,height);
    return this;
}

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    

    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
       DSK_CHILDPROP_I32(left,"x"),
       DSK_CHILDPROP_I32(top,"y"),
    }));

    return exports;
}


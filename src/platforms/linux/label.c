#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>
#include <assert.h>

#include "libdesktop.h"
#define MODULE "label"




LIBUI_FUNCTION(print) {
    INIT_EMPTY_ARGS()
    GtkWidget* widget;
    DSK_UNWRAP_WIDGET();
    // printf("WIDGET UNWRAPPED %p\n", widget);

    return NULL;
}


LIBUI_FUNCTION(labelNew) {
    INIT_ARGS(2);

   	GtkWidget* widget = gtk_label_new("");
    

    GdkRGBA color; 
    gdk_rgba_parse (&color, "rgba(100,90,80,1)");
    gtk_widget_override_background_color(widget,GTK_STATE_FLAG_NORMAL, &color);
    dsk_wrap_widget(env,widget,this);

    if (dsk_set_properties(env, argv[0], this)) {
        napi_throw_error(env,NULL,"Error while setting widget properties.\n");
        return NULL;
    }

    return this;
}

napi_value label_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    

    dsk_define_class(env,module,"Label",labelNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
       DSK_RWPROP_S(label),
       (napi_property_descriptor){.utf8name="setLabel", .method=widgetSetPropS,.data="label"},
       DSK_CHILDPROP_I32(left,"x"),
       DSK_CHILDPROP_I32(top,"y"),
       (napi_property_descriptor) {.method=print,.utf8name="print"},
    }));

    return exports;
}


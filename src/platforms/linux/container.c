#include "napi_utils.h"
#include "control.h"
#include "widget.h"
#include <gtk/gtk.h>

#include <yoga/Yoga.h>

#define MODULE "container"

LIBUI_FUNCTION(containerAppend) {
    INIT_ARGS(1);

   	GtkWidget* widget;
    DSK_UNWRAP_WIDGET();
    
    return this;
}

void dsk_widget_reposition(napi_env env, UIHandle container, UIHandle widget, float xcoord, float ycoord, float width, float height) {
    GValue x = G_VALUE_INIT;
    g_value_init(&x,G_TYPE_INT);
    g_value_set_int(&x,xcoord);
    GValue y = G_VALUE_INIT;
    g_value_init(&y,G_TYPE_INT);
    g_value_set_int(&y,ycoord);
    gtk_container_child_set_property(container, widget, "x", &x);
    gtk_container_child_set_property(container, widget, "y", &y);
    gtk_widget_set_size_request(widget, (int)width,(int)height);
}

void dsk_platform_container_add_child(UIHandle parent, UIHandle child) {
    gtk_container_add(GTK_CONTAINER(parent), GTK_WIDGET(child));
}

LIBUI_FUNCTION(containerNew) {
    INIT_ARGS(2);

   	GtkWidget* widget = gtk_fixed_new();
    dsk_wrap_widget(env, widget, this);                                         

    // printf("argc %d\n",argc);
    if (dsk_set_properties(env, argv[0], this)) {
        napi_throw_error(env,NULL,"Error while setting widget properties.\n");
        return NULL;
    }
    
    dsk_append_all_children(env, widget, argv[1]);

    return this;
}

napi_ref ContainerRef;

napi_value container_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    dsk_define_class_ref(env,module,"Container",containerNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(enabled,"enabled"),
    }), &ContainerRef);

    return exports;
}


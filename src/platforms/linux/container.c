#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

#define MODULE "container"


static void container_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

LIBUI_FUNCTION(containerNew) {
    INIT_ARGS(2);

   	GtkWidget* widget = gtk_fixed_new();
    napi_status status = napi_wrap(env, this, widget, container_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    napi_value children = argv[1];
    uint32_t len;
    napi_get_array_length(env, children, &len);
    printf("container children count %d\n", len);
    for (uint32_t i=0; i < len; i++) {
        printf("add %d\n",i);
        napi_value idx;
        napi_create_uint32(env,i,&idx);
        napi_value child;
        napi_get_property(env,children,idx,&child);
        GtkWidget* child_gtk;
        napi_unwrap(env,child,(void**)&child_gtk);
        gtk_container_add(GTK_CONTAINER(widget), child_gtk);

        /*napi_value x;
        napi_create_uint32(env,0,&x);

        napi_value y;
        napi_create_uint32(env,i*30,&y);
        printf("X:Y = %d:%d\n",0,i*30);
        napi_set_named_property(env,child,"left",x);
        napi_set_named_property(env,child,"top",y);
        */
    }

    return this;
}

LIBUI_FUNCTION(containerAppend) {
    INIT_ARGS(1);

   	GtkWidget* widget;
    DSK_UNWRAP_WIDGET();
    
    return this;
}




napi_value container_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()

    dsk_define_class(env,module,"Container",containerNew,((napi_property_descriptor[]){
       DSK_RWPROP_BOOL(visible,"visible"),
       DSK_RWPROP_BOOL(visible,"enabled"),
    }));

    return exports;
}


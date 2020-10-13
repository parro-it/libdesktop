#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

#define MODULE "win"


static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);

    printf("WIDNWOS NEW\n");
   	GtkWindow* window =(GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    napi_status status = napi_wrap(env, this, window, window_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    uint32_t len;
    napi_get_array_length(env,argv[1],&len);
    printf("child count %d\n",len);
    for (uint32_t i=0; i < len; i++) {
        printf("add %d\n",i);
        napi_value idx;
        napi_create_uint32(env,i,&idx);
        napi_value child;
        napi_get_property(env,argv[1],idx,&child);
        GtkWidget* child_gtk;
        napi_unwrap(env,child,(void**)&child_gtk);

        gtk_container_add(GTK_CONTAINER( window), child_gtk);
    }
    gtk_widget_show_all(GTK_WIDGET(window));
    return this;
}


napi_value win_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    
    dsk_define_class(env, module,"Window",windowNew,((napi_property_descriptor[]) {
        DSK_RWPROP_S(title),
        DSK_RWPROP_I32(width,"default-width"),
        DSK_RWPROP_I32(height,"default-height"),
        DSK_RWPROP_BOOL(visible,"visible"),
    }));
    
    return exports;
}


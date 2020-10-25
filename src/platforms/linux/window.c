#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#include <gtk/gtk.h>

#define MODULE "win"


static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

extern napi_ref ContainerRef;


LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);

    // printf("WINDOWS NEW\n");
   	GtkWindow* window =(GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    dsk_wrap_widget(env, window, this);

    if (dsk_set_properties(env, argv[0], this)) {
        napi_throw_error(env,NULL,"Error while setting widget properties.\n");
        return NULL;
    }

    napi_value Container;
    napi_value container;
    napi_value null;
    napi_value props;

    napi_create_object(env,&props);
    napi_get_reference_value(env, ContainerRef, &Container);
    //napi_get_null(env,&null);
    napi_new_instance(env, Container,2,(napi_value[]){props,argv[1]},&container);
    napi_set_named_property(env, this, "container", container);

    GtkWidget* child_gtk;
    napi_unwrap(env,container,(void**)&child_gtk);
    gtk_container_add(GTK_CONTAINER(window), child_gtk);

    gtk_window_set_position(window,GTK_WIN_POS_CENTER);
    gtk_widget_show_all(GTK_WIDGET(window));

    YGNodeRef root = dsk_widget_get_node(env, container);
    
    dsk_set_children_preferred_sizes(root,child_gtk);
    
    dsk_calculate_layout(env, child_gtk, root);

    
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


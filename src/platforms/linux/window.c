#include "napi_utils.h"
#include "widget.h"
#include "control.h"
#include "libdesktop.h"
#include <gtk/gtk.h>

#define MODULE "win"

/*
static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}
*/
extern napi_ref ContainerRef;


struct on_resize_args {
    GtkWidget* container;
    napi_env env;
};

static bool on_window_resize(GtkWindow *window, GdkEvent *event, gpointer data) {

    struct on_resize_args* args = data;
    napi_env env = args->env;
    char *dsk_error_msg;
    napi_handle_scope handle_scope;
	DSK_NAPI_CALL(napi_open_handle_scope(env, &handle_scope));

    napi_value wrapper = dsk_widget_wrapper(env, args->container);


    YGNodeRef root = dsk_widget_get_node(args->env, wrapper);

    dsk_set_children_preferred_sizes(root, args->container);
   
    int uw;
    int uh;
    gtk_window_get_size(window,&uw,&uh);

    //printf("dsk_calculate_layout %dx%d\n",uw,uh);
   
    dsk_calculate_layout(args->env, args->container, root, (float)uw,(float)uh);


	DSK_NAPI_CALL(napi_close_handle_scope(env, handle_scope));

    return false;

    dsk_error:
        ;
        napi_value uncErr;
        napi_value err_msg;
        napi_create_string_utf8(env, dsk_error_msg, NAPI_AUTO_LENGTH, &err_msg);
        napi_create_error(env,NULL,err_msg,&uncErr);
        napi_fatal_exception(env, uncErr);
        return false;
}


LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);

    // printf("WINDOWS NEW\n");
   	GtkWindow* window =(GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GError* err=NULL;
    gtk_window_set_icon_from_file(
        window, "/home/parroit/Desktop/parro-it/libdesktop/example/icon.png", 
        &err
    );

    gtk_window_set_resizable(window,true);

    if (err != NULL) {
        printf("ERROR %p %s\n",err,"err->message");
        napi_throw_error(env,NULL,err->message);
        return NULL;
    }

    dsk_wrap_widget(env, window, this);

    if (dsk_set_properties(env, argv[0], this)) {
        napi_throw_error(env,NULL,"Error while setting widget properties.\n");
        return NULL;
    }

    napi_value Container;
    napi_value container;
    napi_value props;

    napi_create_object(env,&props);
    napi_get_reference_value(env, ContainerRef, &Container);
    
    bool hasStyle;
    DSK_NAPI_CALL(napi_has_named_property(env,argv[0],"style",&hasStyle));
    if (hasStyle) {
        napi_value containerStyle;
        DSK_NAPI_CALL(napi_get_named_property(env,argv[0],"style",&containerStyle));
        DSK_NAPI_CALL(napi_set_named_property(env,props,"style",containerStyle));
    }

    napi_new_instance(env, Container,2,(napi_value[]){props,argv[1]},&container);
    napi_set_named_property(env, this, "container", container);

    GtkWidget* cntr_gtk;
    napi_unwrap(env,container,(void**)&cntr_gtk);
    gtk_container_add(GTK_CONTAINER(window), cntr_gtk);

    gtk_window_set_position(window,GTK_WIN_POS_CENTER);
    gtk_widget_show_all(GTK_WIDGET(window));

    YGNodeRef root = dsk_widget_get_node(env, container);
    
    dsk_set_children_preferred_sizes(root,cntr_gtk);
    
    dsk_calculate_layout(env, cntr_gtk, root, YGUndefined, YGUndefined);

    float w = YGNodeLayoutGetWidth(root);
    float h = YGNodeLayoutGetHeight(root);
    float pd = YGNodeLayoutGetPadding(root,YGEdgeRight);

    int uw;
    int uh;
    gtk_window_get_size(window,&uw,&uh);

    printf("window: %dx%d layout:%.0fx%.0f\n",uw,uh, w,h);
    
    gtk_window_resize(window,(int)w+pd,(int)h);

    struct on_resize_args* args = malloc(sizeof(struct on_resize_args));
    args->env=env;
    args->container=cntr_gtk;
    g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(on_window_resize), args);

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


#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

#define MODULE "win"


static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

GtkWidget* container_new_gtk();
void append_all_children_gtk(napi_env env,GtkWidget* widget,napi_value children);
void calculate_layout_gtk(GtkWidget* container);

LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);

    printf("WIDNWOS NEW\n");
   	GtkWindow* window =(GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    napi_status status = napi_wrap(env, this, window, window_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    GtkWidget* child_gtk = container_new_gtk();
    append_all_children_gtk(env, child_gtk, argv[1]);
    gtk_container_add(GTK_CONTAINER( window), child_gtk);
    calculate_layout_gtk(child_gtk);

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


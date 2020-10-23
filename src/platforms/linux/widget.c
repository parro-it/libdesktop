#include "napi_utils.h"
#include "widget.h"
#include <gtk/gtk.h>

LIBUI_FUNCTION(widgetSetPropS) {
    INIT_ARGS(1);
    ARG_STRING(value, 0);
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();

    g_object_set(widget, propname, value, NULL);
    //g_object_set(widget, "visible", true, NULL);

    //gtk_widget_show_all(widget);
    return NULL;
}

LIBUI_FUNCTION(widgetGetPropS) {
    INIT_EMPTY_ARGS();
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        
    // printf("w:%p\n",widget);
    char* result;
    g_object_get(widget, propname, &result, NULL);
    // printf("result:%s\n",result);
    if (result == NULL) {
        napi_value null;
        napi_status status = napi_get_null(env,&null);
        CHECK_STATUS_THROW(status, napi_get_null);                                          
        return null;
    }
    return make_utf8_string(env, result);
}

LIBUI_FUNCTION(widgetSetPropI32) {
    INIT_ARGS(1);
    ARG_INT32(value, 0);
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();
    // printf("SET %s %d",propname,value);

    g_object_set(widget, propname, value, NULL);

    return NULL;
}

LIBUI_FUNCTION(widgetGetPropI32) {
    INIT_EMPTY_ARGS();
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        

    int32_t result;
    g_object_get(widget, propname, &result, NULL);

    return make_int32(env, result);
}

LIBUI_FUNCTION(widgetSetChildPropI32) {
    INIT_ARGS(1);
    ARG_INT32(value, 0);
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();
    // printf("SET CHILD %s %d\n",propname,value);

    GtkContainer* container = GTK_CONTAINER(gtk_widget_get_parent(widget));
    if (container==NULL) {
        // printf("CONT NULL\n");
        return NULL;    
    } 
    // printf("CONT %p %s\n",container,propname);
    GValue val = G_VALUE_INIT;
    g_value_init(&val,G_TYPE_INT);
    g_value_set_int(&val,value);
    // printf("gval %d\n",value);
    gtk_container_child_set_property(container, widget, propname, &val);
        // printf("gval %d DONE\n",value);

    return NULL;

}

LIBUI_FUNCTION(widgetGetChildPropI32) {
    INIT_EMPTY_ARGS();
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        

    int32_t result;
    g_object_get(widget, propname, &result, NULL);
    GtkContainer* container = GTK_CONTAINER(gtk_widget_get_parent(widget));
    GValue value;
    gtk_container_child_get_property(container, widget, propname, &value);
    return make_int32(env, g_value_get_int(&value));
}

LIBUI_FUNCTION(widgetSetPropBool) {
    INIT_ARGS(1);
    ARG_BOOL(value, 0);
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();
    // printf("SET %s %d",propname,value);
    g_object_set(widget, propname, value, NULL);

    return NULL;
}

LIBUI_FUNCTION(widgetGetPropBool) {
    INIT_EMPTY_ARGS();
    GtkWidget* widget; 
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        

    bool result;
    g_object_get(widget, propname, &result, NULL);
    return make_bool(env, result);
}
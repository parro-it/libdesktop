#include "napi_utils.h"
#include <gtk/gtk.h>

#define MODULE "win"

static napi_ref WindowRef;

static void window_finalize(napi_env env, void *finalize_data, void *finalize_hint) {

}

LIBUI_FUNCTION(windowNew) {
    INIT_ARGS(2);

    printf("WIDNWOS NEW\n");
   	GtkWidget* widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    napi_status status = napi_wrap(env, this, widget, window_finalize, NULL, NULL);
    CHECK_STATUS_THROW(status, napi_wrap);                                          

    return this;
}

#define DSK_UNWRAP_WIDGET() \
    GtkWidget* widget; \
    {\
    napi_status status = napi_unwrap(env, this, (void**)&widget);\
    CHECK_STATUS_THROW(status, napi_unwrap);                     \
    }

#define LOAD_PROP_NAME() \
    char* propname; \
    {\
    napi_status status = napi_get_cb_info(env, info, NULL, NULL, NULL,&propname); \
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                           \
    }

LIBUI_FUNCTION(widgetSetPropS) {
    INIT_ARGS(1);
    ARG_STRING(value, 0);
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();

    g_object_set(widget, propname, value, NULL);
    //g_object_set(widget, "visible", true, NULL);

    //gtk_widget_show_all(widget);
    return NULL;
}

LIBUI_FUNCTION(widgetGetPropS) {
    INIT_ARGS(0);
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        
    printf("w:%p\n",widget);
    char* result;
    g_object_get(widget, propname, &result, NULL);
    printf("result:%s\n",result);
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
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();
    printf("SET %s %d",propname,value);

    g_object_set(widget, propname, value, NULL);

    return NULL;
}

LIBUI_FUNCTION(widgetGetPropI32) {
    INIT_ARGS(0);
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        

    int32_t result;
    g_object_get(widget, propname, &result, NULL);
    return make_int32(env, result);
}

LIBUI_FUNCTION(widgetSetPropBool) {
    INIT_ARGS(1);
    ARG_BOOL(value, 0);
    DSK_UNWRAP_WIDGET();
    LOAD_PROP_NAME();
    printf("SET %s %d",propname,value);
    g_object_set(widget, propname, value, NULL);

    return NULL;
}

LIBUI_FUNCTION(widgetGetPropBool) {
    INIT_ARGS(0);
    DSK_UNWRAP_WIDGET();                                                                               
    LOAD_PROP_NAME();                                        

    bool result;
    g_object_get(widget, propname, &result, NULL);
    return make_bool(env, result);
}

#define DSK_RWPROP_S(N)  (napi_property_descriptor) {.utf8name = #N, .getter = widgetGetPropS, .setter = widgetSetPropS, .data = #N}
#define DSK_RWPROP_I32(NAME, UI_NAME)  (napi_property_descriptor) {.utf8name = #NAME, .getter = widgetGetPropI32, .setter = widgetSetPropI32, .data = UI_NAME}
#define DSK_RWPROP_BOOL(NAME, UI_NAME)  (napi_property_descriptor) {.utf8name = #NAME, .getter = widgetGetPropBool, .setter = widgetSetPropBool, .data = UI_NAME}

napi_value win_init(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    
    const napi_property_descriptor properties[] = {
        DSK_RWPROP_S(title),
        DSK_RWPROP_I32(width,"default-width"),
        DSK_RWPROP_I32(height,"default-height"),
        DSK_RWPROP_BOOL(visible,"visible"),
    };
    
    napi_status status;
    napi_value Window;

    status = napi_define_class(env, "Window", NAPI_AUTO_LENGTH, windowNew, NULL, 4, properties, &Window);
    CHECK_STATUS_THROW(status, napi_define_class);                                          
	
    status = napi_create_reference(env, Window, 1, &WindowRef);
    CHECK_STATUS_THROW(status, napi_create_reference);                                          

    status = napi_set_named_property(env, module, "Window", Window);                                  
	CHECK_STATUS_THROW(status, napi_set_named_property);   

    return exports;
}


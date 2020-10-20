#include "yoga/Yoga.h"
#include "gtk/gtk.h"
#include "napi_utils.h"

YGNodeRef dsg_widget_get_node(napi_env env, napi_value widget) {
    GObject* widgetG;
    napi_status status = napi_unwrap(env, widget, (void**)&widgetG);
    if (status != napi_ok) {
        return NULL;
    }
    
    YGNodeRef node = g_object_get_data(widgetG,"yoganode");
    return node;
}
#include "yoga.h"
#include "gtk/gtk.h"
#include "napi_utils.h"
#include "control.h"

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget) {
    // printf("unwrap %p\n", widget);
    GObject* widgetG;
    napi_status status = napi_unwrap(env, widget, (void**)&widgetG);
    if (status != napi_ok) {
        // printf("non ok \n");
        const napi_extended_error_info *result;                                                    
		napi_get_last_error_info(env, &result);                                                    
		// printf("napi_unwrap failed with code %d: %s\n", result->engine_error_code, result->error_message);
        return NULL;
    }
    // printf("widgetG %p\n",widgetG);
    YGNodeRef node = g_object_get_data(widgetG,"yoganode");
    return node;
}

void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node) {
    GObject* widgetG;
    napi_status status = napi_unwrap(env, widget, (void**)&widgetG);
    if (status != napi_ok) {
        return;
    }
    
    g_object_set_data(widgetG,"yoganode", node);
    napi_ref ref;
    napi_create_reference(env,widget,1,&ref);

    g_object_set_data(widgetG,"wrapper", ref);
}


napi_value dsk_widget_wrapper(napi_env env, UIHandle widget) {
    napi_ref ref = g_object_get_data(G_OBJECT(widget),"wrapper");
    if (ref == NULL) {
        return NULL;
    }
    // printf("ref is %p\n",ref);
    napi_value node;
    napi_get_reference_value(env,ref,&node);
    return node;
}


void dsk_get_preferred_sizes(UIHandle widget, int* width, int* height) {
    int minwidth,natwidth;
    int minheight,natheight;
    gtk_widget_set_size_request(widget,0,0);
    gtk_widget_get_preferred_height(widget, &minheight, &natheight);
    gtk_widget_get_preferred_width(widget, &minwidth, &natwidth);

    //printf("width min %d nat %d\n", minwidth, natwidth);
    //printf("height min %d nat %d\n", minheight, natheight);
    *width = natwidth;
    *height = natheight;
}

void dsk_connect_event(UIHandle widget, char* eventname, struct dsk_event_args* args) {
    g_signal_connect(G_OBJECT(widget), eventname, G_CALLBACK(on_event), args);
}
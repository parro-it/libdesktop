
#include "napi_utils.h"
#include "control.h"
#include "yoga.h"
#include <windows.h>
#include <WinUser.h>


struct win32_ref {
    void* wrapper;
    void* node;
};

YGNodeRef dsk_widget_get_node(napi_env env, napi_value widget) {
    HWND widgetG;
    napi_status status = napi_unwrap(env, widget, (void**)&widgetG);
    if (status != napi_ok) {
        const napi_extended_error_info *result;                                                    
		napi_get_last_error_info(env, &result);                                                    
        return NULL;
    }
    struct win32_ref* data = (void*)GetWindowLongPtr(widgetG, GWLP_USERDATA);
    // printf("GET NODE %p %p\n", widgetG, data);
    return data->node;
    

}

#include <assert.h>

void dsk_widget_set_node(napi_env env, napi_value widget, YGNodeRef node) {
    HWND widgetG;
    napi_status status = napi_unwrap(env, widget, (void**)&widgetG);
    assert (status == napi_ok);
    assert (widgetG != NULL);

    napi_ref ref;
    napi_create_reference(env,widget,1,&ref);

    struct win32_ref* data = malloc(sizeof(struct win32_ref));
    data->wrapper = ref;
    data->node = node;
    // printf("SET NODE %p %p\n",widgetG,data);
    SetWindowLongPtr(widgetG, GWLP_USERDATA,(LONG_PTR) data);
    
}


napi_value dsk_widget_wrapper(napi_env env, UIHandle widget) {
    struct win32_ref* data = (void*)GetWindowLongPtr((HWND) widget, GWLP_USERDATA);
    napi_ref ref = data->wrapper;
    if (ref == NULL) {
        return NULL;
    }
    napi_value wrapper;
    napi_get_reference_value(env,ref,&wrapper);
    return wrapper;
}
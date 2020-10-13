#ifndef DSG_WIDGET_H__
#define DSG_WIDGET_H__ 1
#include "napi_utils.h"

#define DSK_UNWRAP_WIDGET() \
    {\
    napi_status status = napi_unwrap(env, this, (void**)&widget);\
    CHECK_STATUS_THROW(status, napi_unwrap);                     \
    }

#define LOAD_PROP_NAME() \
    char* propname; \
    {\
    napi_status status = napi_get_cb_info(env, info, NULL, NULL, NULL,(void**)&propname); \
    CHECK_STATUS_THROW(status, napi_get_cb_info);                                           \
    }

#define DSK_RWPROP_S(N)  (napi_property_descriptor) {.utf8name = #N, .getter = widgetGetPropS, .setter = widgetSetPropS, .data = #N}
#define DSK_RWPROP_I32(NAME, UI_NAME)  (napi_property_descriptor) {.utf8name = #NAME, .getter = widgetGetPropI32, .setter = widgetSetPropI32, .data = UI_NAME}
#define DSK_RWPROP_BOOL(NAME, UI_NAME)  (napi_property_descriptor) {.utf8name = #NAME, .getter = widgetGetPropBool, .setter = widgetSetPropBool, .data = UI_NAME}

LIBUI_FUNCTION(widgetSetPropS);
LIBUI_FUNCTION(widgetGetPropS);
LIBUI_FUNCTION(widgetSetPropI32);
LIBUI_FUNCTION(widgetGetPropI32);
LIBUI_FUNCTION(widgetSetPropBool);
LIBUI_FUNCTION(widgetGetPropBool);

napi_value _dsk_define_class(napi_env env, napi_value exports, const char* name, napi_callback constructor,const napi_property_descriptor properties[], size_t propertiesCount);

#define dsk_define_class(env, exports,  name,  constructor, properties) \
    _dsk_define_class(env,exports,name,constructor,(properties), sizeof((properties))/sizeof(napi_property_descriptor))
#endif
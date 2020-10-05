#include "napi_utils.h"
#define MODULE "app"

LIBUI_FUNCTION(appArch);
LIBUI_FUNCTION(createApp) {
    return make_utf8_string(env,"createApp");;
}

napi_value _app_init_core(napi_env env, napi_value exports) {
    DEFINE_MODULE()
    LIBUI_EXPORT(createApp)
    LIBUI_EXPORT(appArch)
    return exports;
}
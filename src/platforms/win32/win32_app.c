#include "napi_utils.h"
#define MODULE "app"

LIBUI_FUNCTION(appArch) {
    //printf("appArch\n");
    
    return make_utf8_string(env,"win32");
}
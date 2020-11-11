#include "libdesktop.h"

DSK_JS_FUNC(dsk_app_getArch) {
	DSK_JS_FUNC_INIT();
	napi_value ver;
	DSK_NAPI_CALL(napi_create_string_utf8(env, "darwin", NAPI_AUTO_LENGTH, &ver));
	return ver;
}
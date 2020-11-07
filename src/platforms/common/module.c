#include "libdesktop.h"
#include <node_api.h>

napi_value win_init(napi_env env, napi_value exports);
napi_value label_init(napi_env env, napi_value exports);
napi_value container_init(napi_env env, napi_value exports);
napi_value textfield_init(napi_env env, napi_value exports);

DSK_USE_MODULE_INITIALIZER(libdesktop);

static napi_value init_all(napi_env env, napi_value exports) {
	label_init(env, exports);
	win_init(env, exports);
	container_init(env, exports);
	textfield_init(env, exports);
	dsk_init_libdesktop(env, exports);
	return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)

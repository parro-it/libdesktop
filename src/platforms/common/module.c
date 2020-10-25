#include "napi_utils.h"

napi_value app_init_core(napi_env env, napi_value exports);
napi_value _libui_init_event_loop(napi_env env, napi_value exports);
napi_value win_init(napi_env env, napi_value exports);
napi_value label_init(napi_env env, napi_value exports);
napi_value container_init(napi_env env, napi_value exports);
napi_value style_init(napi_env env, napi_value exports);
napi_value textfield_init(napi_env env, napi_value exports);
napi_value event_init(napi_env env, napi_value exports);

static napi_value init_all(napi_env env, napi_value exports) {
	app_init_core(env, exports);
	_libui_init_event_loop(env, exports);
	label_init(env, exports);
	win_init(env, exports);
	container_init(env, exports);
	style_init(env, exports);
	textfield_init(env, exports);
	event_init(env, exports);
	return exports;
}


NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)

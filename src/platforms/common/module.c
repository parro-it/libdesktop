#include "napi_utils.h"

napi_value app_init_core(napi_env env, napi_value exports);
napi_value _libui_init_event_loop(napi_env env, napi_value exports);
napi_value win_init(napi_env env, napi_value exports);
napi_value label_init(napi_env env, napi_value exports);
napi_value container_init(napi_env env, napi_value exports);
napi_value style_init(napi_env env, napi_value exports);

static napi_value init_all(napi_env env, napi_value exports) {
	//printf("11\n");
	app_init_core(env, exports);
	//printf("22\n");
	_libui_init_event_loop(env, exports);
	//printf("33\n");
	label_init(env, exports);
	//printf("44\n");
	win_init(env, exports);
	//printf("55\n");
	container_init(env, exports);
	//printf("66\n");
	style_init(env, exports);
	//printf("77\n");
	return exports;
}


NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)

#include "napi_utils.h"

napi_value _app_init_core(napi_env env, napi_value exports);

static napi_value init_all(napi_env env, napi_value exports) {
	_app_init_core(env, exports);

	return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)

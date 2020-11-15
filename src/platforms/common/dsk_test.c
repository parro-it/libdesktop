#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, NativeTests) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(0);
	return this;
}

napi_status dsk_assert(napi_env env, napi_value t, bool assertion) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	napi_value assertTrue;
	DSK_NAPI_CALL(napi_get_named_property(env, t, "true", &assertTrue));
	napi_value ignored;
	napi_value assertion_res;
	DSK_NAPI_CALL(napi_get_boolean(env, assertion, &assertion_res));
	DSK_NAPI_CALL(napi_call_function(env, t, assertTrue, 1, &assertion_res, &ignored));

	return napi_ok;
}

napi_status dsk_end_test(napi_env env, napi_value t) {
	DSK_ONERROR_THROW_RET(napi_pending_exception);

	napi_value end;
	napi_value ignored;
	DSK_NAPI_CALL(napi_get_named_property(env, t, "end", &end));
	DSK_NAPI_CALL(napi_call_function(env, t, end, 0, NULL, &ignored));

	return napi_ok;
}

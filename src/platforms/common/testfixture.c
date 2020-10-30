#include "libdesktop.h"

DSK_DEFINE_MODULE(libdesktop)

DSK_DEFINE_FUNCTION(libdesktop, sum) {
	DSK_JS_FUNC_INIT();

	double a, b;
	napi_value result;

	DSK_NAPI_CALL(napi_get_value_double(env, argv[0], &a));
	DSK_NAPI_CALL(napi_get_value_double(env, argv[1], &b));
	DSK_NAPI_CALL(napi_create_double(env, a + b, &result));

	return result;
}

DSK_DEFINE_FUNCTION(libdesktop, sum_at_least_two) {
	DSK_JS_FUNC_INIT();
	DSK_AT_LEAST_NARGS(2);

	double a, b;
	napi_value result;

	DSK_NAPI_CALL(napi_get_value_double(env, argv[0], &a));
	DSK_NAPI_CALL(napi_get_value_double(env, argv[1], &b));
	DSK_NAPI_CALL(napi_create_double(env, a + b, &result));

	return result;
}

DSK_DEFINE_FUNCTION(libdesktop, sum_exactly_two) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);

	double a, b;
	napi_value result;

	DSK_NAPI_CALL(napi_get_value_double(env, argv[0], &a));
	DSK_NAPI_CALL(napi_get_value_double(env, argv[1], &b));
	DSK_NAPI_CALL(napi_create_double(env, a + b, &result));

	return result;
}

DSK_DEFINE_CLASS(libdesktop, Decimal) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(2);
	/*
	double integral,fractional;

	DSK_NAPI_CALL(napi_get_value_double(env,argv[0],&integral));
	DSK_NAPI_CALL(napi_get_value_double(env,argv[1],&fractional));
*/
	DSK_NAPI_CALL(napi_set_named_property(env, this, "_integral", argv[0]));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "_fractional", argv[1]));

	return this;
}

DSK_JS_FUNC(getIntegral) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(0);

	napi_value _integral;
	DSK_NAPI_CALL(napi_get_named_property(env, this, "_integral", &_integral));

	return _integral;
}

DSK_JS_FUNC(setIntegral) {
	DSK_JS_FUNC_INIT();
	DSK_EXACTLY_NARGS(1);

	uint32_t _integral;
	DSK_NAPI_CALL(napi_get_value_uint32(env, argv[0], &_integral));

	napi_value newval;
	DSK_NAPI_CALL(napi_create_uint32(env, _integral, &newval));
	DSK_NAPI_CALL(napi_set_named_property(env, this, "_integral", newval));

	return NULL;
}

DSK_DEFINE_PROPERTY(libdesktop, Decimal, integral, getIntegral, setIntegral, NULL)
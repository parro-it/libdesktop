#include "napi_utils.h"

napi_value make_bool(napi_env env, bool value) {
	napi_value value_js;

	napi_status status = napi_get_boolean(env, value, &value_js);
	CHECK_STATUS_THROW(status, napi_get_boolean);

	return value_js;
}

napi_value make_double(napi_env env, double value) {
	napi_value value_js;

	napi_status status = napi_create_double(env, value, &value_js);
	CHECK_STATUS_THROW(status, napi_create_double);

	return value_js;
}

napi_value make_int32(napi_env env, int value) {
	napi_value value_js;

	napi_status status = napi_create_int32(env, value, &value_js);
	CHECK_STATUS_THROW(status, napi_create_int32);

	return value_js;
}

napi_value make_int64(napi_env env, long value) {
	napi_value value_js;

	napi_status status = napi_create_int64(env, value, &value_js);
	CHECK_STATUS_THROW(status, napi_create_int64);

	return value_js;
}

napi_value make_uint32(napi_env env, unsigned int value) {
	napi_value value_js;

	napi_status status = napi_create_uint32(env, value, &value_js);
	CHECK_STATUS_THROW(status, napi_create_uint32);

	return value_js;
}

napi_value make_utf8_string(napi_env env, const char *char_ptr) {
	napi_value result;

	napi_status status = napi_create_string_utf8(env, char_ptr, NAPI_AUTO_LENGTH, &result);
	CHECK_STATUS_THROW(status, napi_create_string_utf8);

	return result;
}

napi_value _dsk_define_class(napi_env env, napi_value exports, const char *name,
							 napi_callback constructor, const napi_property_descriptor properties[],
							 size_t propertiesCount, napi_ref *ref) {

	napi_status status;
	napi_value Class;
	// printf("PROPERTIES %s %zd\n",name, propertiesCount);
	status = napi_define_class(env, name, NAPI_AUTO_LENGTH, constructor, NULL, propertiesCount,
							   properties, &Class);
	CHECK_STATUS_THROW(status, napi_define_class);

	napi_ref ignored_ref;
	if (ref == NULL) {
		ref = &ignored_ref;
	}

	status = napi_create_reference(env, Class, 1, ref);
	CHECK_STATUS_THROW(status, napi_create_reference);

	status = napi_set_named_property(env, exports, name, Class);
	CHECK_STATUS_THROW(status, napi_set_named_property);

	return exports;
}
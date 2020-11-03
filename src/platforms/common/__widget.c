#include "__widget.h"
#include "napi_utils.h"

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
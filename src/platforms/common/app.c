#include "napi_utils.h"
#define MODULE "app"

static napi_ref AppRef;

LIBUI_FUNCTION(appArch);

LIBUI_FUNCTION(appVer) {
	return make_utf8_string(env, "0.0.1");
}

LIBUI_FUNCTION(createApp) {
	napi_value instance;
	napi_value args;

	napi_value cons;
	napi_status status;
	status = napi_get_reference_value(env, AppRef, &cons);
	CHECK_STATUS_THROW(status, napi_get_reference_value);

	status = napi_new_instance(env, cons, 0, &args, &instance);
	CHECK_STATUS_THROW(status, napi_new_instance);

	return instance;
}

LIBUI_FUNCTION(newApp) {
	return NULL;
}

napi_value app_init_core(napi_env env, napi_value exports) {
	DEFINE_MODULE()

	const napi_property_descriptor properties[] = {
		(napi_property_descriptor){.utf8name = "arch", .getter = appArch},
		(napi_property_descriptor){.utf8name = "ver", .getter = appVer},
		(napi_property_descriptor){
			.utf8name = "create", .method = createApp, .attributes = napi_static},
	};

	napi_status status;
	napi_value App;

	status = napi_define_class(env, "App", NAPI_AUTO_LENGTH, newApp, NULL, 3, properties, &App);
	CHECK_STATUS_THROW(status, napi_define_class);

	status = napi_create_reference(env, App, 1, &AppRef);
	CHECK_STATUS_THROW(status, napi_create_reference);

	status = napi_set_named_property(env, module, "App", App);
	CHECK_STATUS_THROW(status, napi_set_named_property);

	return exports;
}
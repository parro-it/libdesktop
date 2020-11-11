#include "libdesktop-host.h"
#include "libdesktop.h"

DSK_EXTEND_MODULE(libdesktop);

DSK_DEFINE_CLASS(libdesktop, App) {
	DSK_JS_FUNC_INIT();
	const char *err = dsk_init();
	if (err != NULL) {
		napi_throw_error(env, NULL, err);
		free((void *)err);
	}
	ln_init_loop_status();
	return this;
}

DSK_JS_FUNC(dsk_app_getArch);

static DSK_JS_FUNC(getVer) {
	DSK_JS_FUNC_INIT();
	napi_value ver;
	DSK_NAPI_CALL(napi_create_string_utf8(env, "0.0.1", NAPI_AUTO_LENGTH, &ver));
	return ver;
}

DSK_DEFINE_PROPERTY(libdesktop, App, arch, dsk_app_getArch, NULL, NULL)
DSK_DEFINE_PROPERTY(libdesktop, App, ver, getVer, NULL, NULL)

DSK_DEFINE_STATIC_METHOD(libdesktop, App, create) {
	napi_value instance = dsk_new_instance(env, libdesktop_App_ref, 0, NULL);
	return instance;
}

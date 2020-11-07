#include "libdesktop.h"

DSK_JS_FUNC(dsk_app_getArch) {
	//// printf("appArch\n");

	return make_utf8_string(env, "linux");
}
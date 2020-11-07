#include "libdesktop.h"

DSK_JS_FUNC(dsk_app_getArch) {
	return make_utf8_string(env, "darwin");
}